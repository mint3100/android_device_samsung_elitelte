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

PRODUCT_COPY_FILES += \
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
