DEVICE_PATH := device/samsung/elitelte_kor

PRODUCT_AAPT_CONFIG := normal
PRODUCT_AAPT_PREF_CONFIG := mdpi
PRODUCT_CHARACTERISTICS := phone

PRODUCT_PROPERTY_OVERRIDES += \
    ro.hardware=qcom \
    ro.sf.lcd_density=240 \
    persist.sys.usb.config=mtp,adb \
    debug.sf.hw=1 \
    debug.egl.hw=1 \
    persist.cne.feature=1 \
    persist.fuse_sdcard=true \
    persist.timed.enable=true \
    persist.hwc.mdpcomp.enable=true \
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
    $(DEVICE_PATH)/rootdir/init.rilcommon.rc:root/init.rilcommon.rc \
    $(DEVICE_PATH)/rootdir/init.target.rc:root/init.target.rc \
    $(DEVICE_PATH)/rootdir/init.trace.rc:root/init.trace.rc \
    $(DEVICE_PATH)/rootdir/ueventd.qcom.rc:root/ueventd.qcom.rc \
    $(DEVICE_PATH)/recovery.fstab:root/etc/recovery.fstab

$(call inherit-product-if-exists, vendor/samsung/elitelte_kor/elitelte_kor-vendor.mk)
