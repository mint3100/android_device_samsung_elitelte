LOCAL_PATH := $(call my-dir)
DEVICE_LOCAL_PATH := $(LOCAL_PATH)

LOCAL_PATH := $(DEVICE_LOCAL_PATH)
include $(DEVICE_LOCAL_PATH)/gatekeeper/Android.mk

LOCAL_PATH := $(DEVICE_LOCAL_PATH)

include $(CLEAR_VARS)
LOCAL_MODULE := init.qcom.modem_links.sh
LOCAL_MODULE_CLASS := ETC
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE_PATH := $(TARGET_OUT_ETC)
LOCAL_SRC_FILES := rootdir/init.qcom.modem_links.sh
LOCAL_POST_INSTALL_CMD := \
    mkdir -p $(TARGET_OUT_ETC)/firmware; \
    for img in \
        modem.mdt modem.b00 modem.b01 modem.b02 modem.b03 modem.b04 modem.b05 \
        modem.b06 modem.b07 modem.b08 modem.b09 modem.b10 modem.b11 modem.b12 \
        modem.b13 modem.b14 modem.b15 modem.b16 modem.b17 modem.b18 modem.b19 \
        modem.b20 modem.b21 modem.b22 modem.b23 modem.b24 modem.b25 modem.b26 \
        modem.b27 modem.b28 modem.b29 modem.b30 \
        adsp.mdt adsp.b00 adsp.b01 adsp.b02 adsp.b03 adsp.b04 adsp.b05 \
        adsp.b06 adsp.b07 adsp.b08 adsp.b09 adsp.b10 adsp.b11 adsp.b12 \
        adsp.b13 adsp.b14 adsp.b15 adsp.b16 adsp.b17 adsp.b18 adsp.b19 \
        adsp.b20 adsp.b21 adsp.b22 adsp.b23 adsp.b24 adsp.b25 adsp.b26 \
        adsp.b27 adsp.b28 adsp.b29 adsp.b30 \
        mba.mbn mba.mdt; do \
        ln -sf /firmware-modem/image/$$img $(TARGET_OUT_ETC)/firmware/$$img; \
    done
include $(BUILD_PREBUILT)
