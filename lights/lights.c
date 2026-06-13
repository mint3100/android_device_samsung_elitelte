#include <errno.h>
#include <fcntl.h>
#include <hardware/hardware.h>
#include <hardware/lights.h>
#include <log/log.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))

static const char kBacklightPath[] = "/sys/class/leds/lcd-backlight/brightness";
static const char kBacklightMaxPath[] = "/sys/class/leds/lcd-backlight/max_brightness";
static const char kKeypadPath[] = "/sys/class/sec/sec_keypad/brightness";

struct rgb_led {
    const char *brightness;
    const char *max_brightness;
    const char *trigger;
    const char *delay_on;
    const char *delay_off;
    const char *blink;
};

static const struct rgb_led kRgbLeds[] = {
    {
        "/sys/class/leds/led_r/brightness",
        "/sys/class/leds/led_r/max_brightness",
        "/sys/class/leds/led_r/trigger",
        "/sys/class/leds/led_r/delay_on",
        "/sys/class/leds/led_r/delay_off",
        "/sys/class/leds/led_r/blink",
    },
    {
        "/sys/class/leds/led_g/brightness",
        "/sys/class/leds/led_g/max_brightness",
        "/sys/class/leds/led_g/trigger",
        "/sys/class/leds/led_g/delay_on",
        "/sys/class/leds/led_g/delay_off",
        "/sys/class/leds/led_g/blink",
    },
    {
        "/sys/class/leds/led_b/brightness",
        "/sys/class/leds/led_b/max_brightness",
        "/sys/class/leds/led_b/trigger",
        "/sys/class/leds/led_b/delay_on",
        "/sys/class/leds/led_b/delay_off",
        "/sys/class/leds/led_b/blink",
    },
};

static pthread_mutex_t g_lock = PTHREAD_MUTEX_INITIALIZER;
static struct light_state_t g_battery;
static struct light_state_t g_notification;
static int g_last_rgb_values[ARRAY_SIZE(kRgbLeds)] = { -1, -1, -1 };

static int write_str(const char *path, const char *value)
{
    int fd = open(path, O_WRONLY);
    if (fd < 0) {
        return -errno;
    }

    ssize_t len = (ssize_t)strlen(value);
    ssize_t ret = write(fd, value, len);
    int err = ret == len ? 0 : -errno;
    close(fd);
    return err;
}

static int write_int(const char *path, int value)
{
    char buffer[32];
    snprintf(buffer, sizeof(buffer), "%d\n", value);
    return write_str(path, buffer);
}

static int read_int(const char *path, int fallback)
{
    char buffer[32];
    int fd = open(path, O_RDONLY);
    if (fd < 0) {
        return fallback;
    }

    ssize_t ret = read(fd, buffer, sizeof(buffer) - 1);
    close(fd);
    if (ret <= 0) {
        return fallback;
    }

    buffer[ret] = '\0';
    int value = atoi(buffer);
    return value > 0 ? value : fallback;
}

static int rgb_brightness(const struct light_state_t *state)
{
    int color = state->color & 0x00ffffff;
    int red = (color >> 16) & 0xff;
    int green = (color >> 8) & 0xff;
    int blue = color & 0xff;

    return (77 * red + 150 * green + 29 * blue) >> 8;
}

static bool is_lit(const struct light_state_t *state)
{
    return (state->color & 0x00ffffff) != 0;
}

static int scale_brightness(int value, int max)
{
    if (value <= 0) {
        return 0;
    }
    if (value >= 255) {
        return max;
    }
    return (value * max + 127) / 255;
}

static int set_backlight_locked(const struct light_state_t *state)
{
    int brightness = rgb_brightness(state);
    int max = read_int(kBacklightMaxPath, 255);

    return write_int(kBacklightPath, scale_brightness(brightness, max));
}

static int set_keypad_locked(const struct light_state_t *state)
{
    return write_int(kKeypadPath, rgb_brightness(state) > 0 ? 1 : 0);
}

static int set_rgb_led_locked(const struct rgb_led *led, int value,
                              bool blink, int on_ms, int off_ms)
{
    (void)blink;
    (void)on_ms;
    (void)off_ms;

    int max = read_int(led->max_brightness, 255);
    int scaled = scale_brightness(value, max);
    int err = 0;

    if (scaled == 0) {
        write_str(led->trigger, "none\n");
        write_int(led->blink, 0);
        return write_int(led->brightness, 0);
    }

    /*
     * The KTD2026 timer path updates RGB LED registers periodically on this
     * board and has been observed together with panel flicker. Keep the
     * external notification LED solid until the stock blink timing can be
     * reproduced without disturbing display power.
     */
    write_str(led->trigger, "none\n");
    write_int(led->blink, 0);

    int brightness_err = write_int(led->brightness, scaled);
    return brightness_err < 0 ? brightness_err : err;
}

static int set_speaker_light_locked(void)
{
    const struct light_state_t *state =
            is_lit(&g_notification) ? &g_notification : &g_battery;
    int color = state->color & 0x00ffffff;
    int values[ARRAY_SIZE(kRgbLeds)] = {
        (color >> 16) & 0xff,
        (color >> 8) & 0xff,
        color & 0xff,
    };
    bool blink = state->flashMode == LIGHT_FLASH_TIMED &&
            state->flashOnMS > 0 && state->flashOffMS > 0;
    int err = 0;

    bool changed = false;
    for (size_t i = 0; i < ARRAY_SIZE(kRgbLeds); ++i) {
        if (g_last_rgb_values[i] != values[i]) {
            changed = true;
            break;
        }
    }
    if (!changed) {
        return 0;
    }

    for (size_t i = 0; i < ARRAY_SIZE(kRgbLeds); ++i) {
        int led_err = set_rgb_led_locked(&kRgbLeds[i], values[i], blink,
                                         state->flashOnMS, state->flashOffMS);
        if (led_err < 0 && err == 0) {
            err = led_err;
        }
    }

    if (err == 0) {
        for (size_t i = 0; i < ARRAY_SIZE(kRgbLeds); ++i) {
            g_last_rgb_values[i] = values[i];
        }
    }

    return err;
}

static int set_light_backlight(struct light_device_t *dev,
                               const struct light_state_t *state)
{
    pthread_mutex_lock(&g_lock);
    int err = set_backlight_locked(state);
    pthread_mutex_unlock(&g_lock);
    return err;
}

static int set_light_buttons(struct light_device_t *dev,
                             const struct light_state_t *state)
{
    pthread_mutex_lock(&g_lock);
    int err = set_keypad_locked(state);
    pthread_mutex_unlock(&g_lock);
    return err;
}

static int set_light_battery(struct light_device_t *dev,
                             const struct light_state_t *state)
{
    pthread_mutex_lock(&g_lock);
    g_battery = *state;
    int err = set_speaker_light_locked();
    pthread_mutex_unlock(&g_lock);
    return err;
}

static int set_light_notifications(struct light_device_t *dev,
                                   const struct light_state_t *state)
{
    pthread_mutex_lock(&g_lock);
    g_notification = *state;
    int err = set_speaker_light_locked();
    pthread_mutex_unlock(&g_lock);
    return err;
}

static int set_light_attention(struct light_device_t *dev,
                               const struct light_state_t *state)
{
    return set_light_notifications(dev, state);
}

static int close_lights(struct light_device_t *dev)
{
    free(dev);
    return 0;
}

static int open_lights(const struct hw_module_t *module, const char *name,
                       struct hw_device_t **device)
{
    int (*set_light)(struct light_device_t *dev,
                     const struct light_state_t *state);

    if (strcmp(LIGHT_ID_BACKLIGHT, name) == 0) {
        set_light = set_light_backlight;
    } else if (strcmp(LIGHT_ID_BUTTONS, name) == 0 ||
               strcmp(LIGHT_ID_KEYBOARD, name) == 0) {
        set_light = set_light_buttons;
    } else if (strcmp(LIGHT_ID_BATTERY, name) == 0) {
        set_light = set_light_battery;
    } else if (strcmp(LIGHT_ID_NOTIFICATIONS, name) == 0) {
        set_light = set_light_notifications;
    } else if (strcmp(LIGHT_ID_ATTENTION, name) == 0) {
        set_light = set_light_attention;
    } else {
        return -EINVAL;
    }

    struct light_device_t *dev = calloc(1, sizeof(*dev));
    if (dev == NULL) {
        return -ENOMEM;
    }

    dev->common.tag = HARDWARE_DEVICE_TAG;
    dev->common.version = 0;
    dev->common.module = (struct hw_module_t *)module;
    dev->common.close = (int (*)(struct hw_device_t *))close_lights;
    dev->set_light = set_light;

    *device = (struct hw_device_t *)dev;
    return 0;
}

static struct hw_module_methods_t lights_module_methods = {
    .open = open_lights,
};

struct hw_module_t HAL_MODULE_INFO_SYM = {
    .tag = HARDWARE_MODULE_TAG,
    .version_major = 1,
    .version_minor = 0,
    .id = LIGHTS_HARDWARE_MODULE_ID,
    .name = "elitelte lights HAL",
    .author = "LineageOS",
    .methods = &lights_module_methods,
};
