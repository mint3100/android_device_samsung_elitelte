DEVICE_PATH := device/samsung/elitelte

PRODUCT_AAPT_CONFIG := normal
PRODUCT_AAPT_PREF_CONFIG := mdpi
PRODUCT_CHARACTERISTICS := phone

PRODUCT_PACKAGE_OVERLAYS += $(DEVICE_PATH)/overlay

PRODUCT_PROPERTY_OVERRIDES += \
    ro.hardware=qcom \
    ro.board.platform=msm8937 \
    ro.hardware.copybit=msm8937 \
    ro.hardware.egl=adreno \
    ro.hardware.gralloc=msm8937 \
    ro.hardware.hwcomposer=msm8937 \
    ro.hardware.memtrack=msm8937 \
    ro.sf.lcd_density=240 \
    persist.sys.usb.config=mtp,adb \
    rild.libpath=/vendor/lib/libsec-ril.so \
    debug.egl.hw=1 \
    debug.sf.hw=1 \
    debug.gralloc.enable_fb_ubwc=0 \
    debug.gralloc.gfx_ubwc_disable=1 \
    debug.sf.latch_unsignaled=1 \
    persist.cne.feature=1 \
    persist.fuse_sdcard=true \
    persist.sys.sdcardfs=force_off \
    ro.sys.sdcardfs=false \
    persist.hwc.enable_vds=1 \
    persist.timed.enable=true \
    persist.hwc.mdpcomp.enable=true \
    sdm.boot_anim_layer_count=999 \
    vendor.display.disable_skip_validate=1 \
    vendor.display.enable_default_color_mode=1 \
    ro.opengles.version=196608 \
    ro.qc.sdk.audio.ssr=false \
    ro.qc.sdk.audio.fluencetype=none

PRODUCT_DEFAULT_PROPERTY_OVERRIDES += \
    ro.adb.secure=0 \
    ro.secure=0 \
    ro.allow.mock.location=0

PRODUCT_SYSTEM_DEFAULT_PROPERTIES += \
    ro.adb.secure=0 \
    persist.sys.usb.config=adb

PRODUCT_PACKAGES += \
    android.hardware.configstore@1.1-service \
    android.hardware.audio@2.0-impl \
    android.hardware.audio@2.0-service \
    android.hardware.audio.effect@2.0-impl \
    audio.a2dp.default \
    audio.r_submix.default \
    audio.usb.default \
    android.hardware.sensors@1.0-impl \
    android.hardware.sensors@1.0-service \
    android.hardware.health@1.0-impl \
    android.hardware.health@1.0-service \
    android.hardware.gatekeeper@1.0-impl \
    gatekeeper.qcom \
    android.hardware.bluetooth@1.0-impl \
    android.hardware.bluetooth@1.0-service \
    libbt-vendor \
    android.hardware.wifi@1.0-service \
    libtinycompress \
    libxml2 \
    copybit.msm8937 \
    gralloc.msm8937 \
    hwcomposer.msm8937 \
    android.hardware.graphics.allocator@2.0-impl \
    android.hardware.graphics.allocator@2.0-service \
    android.hardware.graphics.mapper@2.0-impl \
    android.hardware.keymaster@3.0-impl \
    android.hardware.keymaster@3.0-service \
    android.hardware.memtrack@1.0-impl \
    android.hardware.memtrack@1.0-service \
    android.hardware.media.omx@1.0-service \
    libmemalloc \
    libqdMetaData \
    libqdutils \
    libqservice \
    libsdmcore \
    libsdmutils \
    memtrack.msm8937 \
    Snap \
    hostapd \
    libcld80211 \
    libwpa_client \
    wificond \
    wpa_supplicant

PRODUCT_COPY_FILES += \
    frameworks/native/data/etc/android.hardware.bluetooth.xml:system/etc/permissions/android.hardware.bluetooth.xml \
    frameworks/native/data/etc/android.hardware.bluetooth_le.xml:system/etc/permissions/android.hardware.bluetooth_le.xml \
    frameworks/native/data/etc/android.hardware.camera.flash-autofocus.xml:system/etc/permissions/android.hardware.camera.flash-autofocus.xml \
    frameworks/native/data/etc/android.hardware.camera.front.xml:system/etc/permissions/android.hardware.camera.front.xml \
    frameworks/native/data/etc/android.hardware.location.gps.xml:system/etc/permissions/android.hardware.location.gps.xml \
    frameworks/native/data/etc/android.hardware.sensor.accelerometer.xml:system/etc/permissions/android.hardware.sensor.accelerometer.xml \
    frameworks/native/data/etc/android.hardware.sensor.light.xml:system/etc/permissions/android.hardware.sensor.light.xml \
    frameworks/native/data/etc/android.hardware.sensor.proximity.xml:system/etc/permissions/android.hardware.sensor.proximity.xml \
    frameworks/native/data/etc/android.hardware.telephony.cdma.xml:system/etc/permissions/android.hardware.telephony.cdma.xml \
    frameworks/native/data/etc/android.hardware.telephony.gsm.xml:system/etc/permissions/android.hardware.telephony.gsm.xml \
    frameworks/native/data/etc/android.hardware.touchscreen.multitouch.jazzhand.xml:system/etc/permissions/android.hardware.touchscreen.multitouch.jazzhand.xml \
    frameworks/native/data/etc/android.hardware.usb.accessory.xml:system/etc/permissions/android.hardware.usb.accessory.xml \
    frameworks/native/data/etc/android.hardware.usb.host.xml:system/etc/permissions/android.hardware.usb.host.xml \
    frameworks/native/data/etc/android.hardware.wifi.direct.xml:system/etc/permissions/android.hardware.wifi.direct.xml \
    frameworks/native/data/etc/android.hardware.wifi.xml:system/etc/permissions/android.hardware.wifi.xml \
    frameworks/native/data/etc/android.software.midi.xml:system/etc/permissions/android.software.midi.xml \
    frameworks/native/data/etc/android.software.sip.voip.xml:system/etc/permissions/android.software.sip.voip.xml \
    frameworks/native/data/etc/handheld_core_hardware.xml:system/etc/permissions/handheld_core_hardware.xml \
    $(DEVICE_PATH)/keylayout/ft5x06_ts.kl:system/usr/keylayout/ft5x06_ts.kl \
    $(DEVICE_PATH)/keylayout/gpio-keys.kl:system/usr/keylayout/gpio-keys.kl \
    $(DEVICE_PATH)/keylayout/qpnp_pon.kl:system/usr/keylayout/qpnp_pon.kl \
    $(DEVICE_PATH)/keylayout/sec_keypad_3x4-keypad.kl:system/usr/keylayout/sec_keypad_3x4-keypad.kl \
    $(DEVICE_PATH)/keylayout/synaptics_dsx.kl:system/usr/keylayout/synaptics_dsx.kl \
    $(DEVICE_PATH)/keylayout/synaptics_rmi4_i2c.kl:system/usr/keylayout/synaptics_rmi4_i2c.kl \
    $(DEVICE_PATH)/rootdir/adb_keys:root/adb_keys \
    $(DEVICE_PATH)/rootdir/fstab.qcom:root/fstab.qcom \
    $(DEVICE_PATH)/rootdir/init.carrier.rc:root/init.carrier.rc \
    $(DEVICE_PATH)/rootdir/init.class_main.sh:root/init.class_main.sh \
    $(DEVICE_PATH)/rootdir/init.container.rc:root/init.container.rc \
    $(DEVICE_PATH)/rootdir/init.mdm.sh:root/init.mdm.sh \
    $(DEVICE_PATH)/rootdir/init.msm.usb.configfs.rc:root/init.msm.usb.configfs.rc \
    $(DEVICE_PATH)/rootdir/init.qcom.class_core.sh:root/init.qcom.class_core.sh \
    $(DEVICE_PATH)/rootdir/init.qcom.early_boot.sh:root/init.qcom.early_boot.sh \
    $(DEVICE_PATH)/rootdir/init.qcom.factory.rc:root/init.qcom.factory.rc \
    $(DEVICE_PATH)/rootdir/init.qcom.rc:root/init.qcom.rc \
    $(DEVICE_PATH)/rootdir/init.qcom.sensors.sh:root/init.qcom.sensors.sh \
    $(DEVICE_PATH)/rootdir/init.qcom.sh:root/init.qcom.sh \
    $(DEVICE_PATH)/rootdir/init.qcom.syspart_fixup.sh:root/init.qcom.syspart_fixup.sh \
    $(DEVICE_PATH)/rootdir/init.qcom.usb.rc:root/init.qcom.usb.rc \
    $(DEVICE_PATH)/rootdir/init.qcom.usb.sh:root/init.qcom.usb.sh \
    $(DEVICE_PATH)/rootdir/init.recovery.qcom.rc:root/init.recovery.qcom.rc \
    $(DEVICE_PATH)/rootdir/init.rilchip.rc:root/init.rilchip.rc \
    $(DEVICE_PATH)/rootdir/init.target.rc:root/init.target.rc \
    $(DEVICE_PATH)/rootdir/init.trace.rc:root/init.trace.rc \
    $(DEVICE_PATH)/rootdir/ueventd.qcom.rc:root/ueventd.qcom.rc \
    vendor/samsung/elitelte/proprietary/etc/firmware/a300_pfp.fw:root/firmware/image/a300_pfp.fw \
    vendor/samsung/elitelte/proprietary/etc/firmware/a300_pm4.fw:root/firmware/image/a300_pm4.fw \
    vendor/samsung/elitelte/proprietary/etc/firmware/a300_pfp.fw:system/vendor/firmware/a300_pfp.fw \
    vendor/samsung/elitelte/proprietary/etc/firmware/a300_pm4.fw:system/vendor/firmware/a300_pm4.fw \
    $(DEVICE_PATH)/recovery.fstab:root/etc/recovery.fstab

$(call inherit-product-if-exists, vendor/samsung/elitelte/elitelte-vendor.mk)

# Qualcomm CAF media stack for MSM8937 hardware codecs.
PRODUCT_PACKAGES += \
    libOmxCore \
    libmm-omxcore \
    libOmxVidcCommon \
    libOmxVenc \
    libOmxVdec \
    libOmxSwVdec \
    libstagefrighthw \
    libc2dcolorconvert
