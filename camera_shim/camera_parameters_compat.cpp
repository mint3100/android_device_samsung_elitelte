#include <dlfcn.h>
#include <stdint.h>
#include <stdlib.h>

namespace android {

class CameraParameters {
public:
    int64_t getInt64(const char *key) const;

    static const char KEY_RAW_CAPTURE[];
    static const char KEY_OIS[];
    static const char KEY_OIS_SUPPORTED[];
    static const char KEY_SUPPORTED_OIS_MODES[];
    static const char OIS_OFF[];
    static const char OIS_ON_STILL[];
    static const char OIS_ON_VIDEO[];
    static const char OIS_ON_ZOOM[];
    static const char OIS_ON_SINE_X[];
    static const char OIS_ON_SINE_Y[];
    static const char OIS_CENTERING[];
    static const char PIXEL_FORMAT_YUV420SP_NV21[];
};

const char CameraParameters::KEY_RAW_CAPTURE[] = "raw-capture";
const char CameraParameters::KEY_OIS[] = "ois";
const char CameraParameters::KEY_OIS_SUPPORTED[] = "ois-supported";
const char CameraParameters::KEY_SUPPORTED_OIS_MODES[] = "ois-mode-values";
const char CameraParameters::OIS_OFF[] = "off";
const char CameraParameters::OIS_ON_STILL[] = "still";
const char CameraParameters::OIS_ON_VIDEO[] = "video";
const char CameraParameters::OIS_ON_ZOOM[] = "zoom";
const char CameraParameters::OIS_ON_SINE_X[] = "sine_x";
const char CameraParameters::OIS_ON_SINE_Y[] = "sine_y";
const char CameraParameters::OIS_CENTERING[] = "center";
const char CameraParameters::PIXEL_FORMAT_YUV420SP_NV21[] = "nv21";

int64_t CameraParameters::getInt64(const char *key) const
{
    typedef const char *(*GetFn)(const CameraParameters *, const char *);
    static GetFn get =
            reinterpret_cast<GetFn>(dlsym(RTLD_DEFAULT, "_ZNK7android16CameraParameters3getEPKc"));

    if (get == 0) {
        return -1;
    }

    const char *value = get(this, key);
    if (value == 0) {
        return -1;
    }

    return strtoll(value, 0, 0);
}

}  // namespace android
