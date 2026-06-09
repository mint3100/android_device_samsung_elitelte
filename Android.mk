LOCAL_PATH := $(call my-dir)
DEVICE_LOCAL_PATH := $(LOCAL_PATH)

# Build the Oreo-era Qualcomm display HALs from source for msm8937.
# The stock Marshmallow hwcomposer blob initializes but crashes
# SurfaceFlinger on LineageOS 15.1, so keep the display stack in the
# normal platform build flow while this device tree owns the opt-in.

include $(CLEAR_VARS)
include hardware/qcom/display/msm8996/common.mk

LOCAL_MODULE := libsdmutils
LOCAL_PROPRIETARY_MODULE := true
LOCAL_MODULE_TAGS := optional
LOCAL_C_INCLUDES := \
    hardware/qcom/display/msm8996/sdm/include
LOCAL_CFLAGS := \
    -Wno-missing-field-initializers \
    -Wno-unused-parameter \
    -std=c++11 \
    -DLOG_TAG=\"SDM\" \
    -DDEBUG_CALC_FPS \
    -D__STDC_FORMAT_MACROS
LOCAL_CLANG := true
LOCAL_SRC_FILES := \
    sdmutils/debug.cpp \
    sdmutils/rect.cpp \
    sdmutils/sys.cpp \
    sdmutils/formats.cpp \
    sdm_legacy_sys.cpp
include $(BUILD_SHARED_LIBRARY)

include $(CLEAR_VARS)
include hardware/qcom/display/msm8996/common.mk

LOCAL_PATH := $(DEVICE_LOCAL_PATH)

LOCAL_MODULE := libsdmcore
LOCAL_PROPRIETARY_MODULE := true
LOCAL_MODULE_TAGS := optional
LOCAL_C_INCLUDES := \
    $(common_includes) \
    $(kernel_includes) \
    $(DEVICE_LOCAL_PATH)/sdmcore
LOCAL_CFLAGS := \
    -Wno-missing-field-initializers \
    -Wno-unused-parameter \
    -Wno-non-virtual-dtor \
    -std=c++11 \
    -fcolor-diagnostics \
    -DLOG_TAG=\"SDM\" \
    $(common_flags) \
    -include $(DEVICE_LOCAL_PATH)/sdm_mdp_compat.h
LOCAL_CLANG := true
LOCAL_SHARED_LIBRARIES := \
    libdl \
    libsdmutils \
    libc++
LOCAL_ADDITIONAL_DEPENDENCIES := $(common_deps) $(kernel_deps)
LOCAL_SRC_FILES := \
    sdmcore/core_interface.cpp \
    sdmcore/core_impl.cpp \
    sdmcore/display_base.cpp \
    sdmcore/display_primary.cpp \
    sdmcore/display_hdmi.cpp \
    sdmcore/display_virtual.cpp \
    sdmcore/comp_manager.cpp \
    sdmcore/strategy.cpp \
    sdmcore/resource_default.cpp \
    sdmcore/dump_impl.cpp \
    sdmcore/color_manager.cpp \
    sdmcore/fb/hw_info.cpp \
    sdmcore/fb/hw_device.cpp \
    sdmcore/fb/hw_primary.cpp \
    sdmcore/fb/hw_hdmi.cpp \
    sdmcore/fb/hw_virtual.cpp \
    sdmcore/fb/hw_color_manager.cpp \
    sdmcore/fb/hw_scale.cpp \
    sdmcore/fb/hw_events.cpp
include $(BUILD_SHARED_LIBRARY)

include $(CLEAR_VARS)
include hardware/qcom/display/msm8996/common.mk

LOCAL_MODULE := hwcomposer.$(TARGET_BOARD_PLATFORM)
LOCAL_MODULE_RELATIVE_PATH := hw
LOCAL_PROPRIETARY_MODULE := true
LOCAL_MODULE_TAGS := optional
LOCAL_C_INCLUDES := \
    $(common_includes) \
    $(DEVICE_LOCAL_PATH)/hwc
LOCAL_CFLAGS := \
    -Wno-missing-field-initializers \
    -Wno-unused-parameter \
    -std=c++11 \
    -fcolor-diagnostics \
    -DLOG_TAG=\"SDM\" \
    $(common_flags)
LOCAL_CLANG := true
LOCAL_SHARED_LIBRARIES := \
    libsdmcore \
    libqservice \
    libbinder \
    libhardware \
    libhardware_legacy \
    libutils \
    liblog \
    libcutils \
    libsync \
    libmemalloc \
    libqdutils \
    libdl \
    libpowermanager \
    libsdmutils \
    libc++
LOCAL_SRC_FILES := \
    hwc/hwc_session.cpp \
    hwc/hwc_display.cpp \
    hwc/hwc_display_null.cpp \
    hwc/hwc_display_primary.cpp \
    hwc/hwc_display_external.cpp \
    hwc/hwc_display_virtual.cpp \
    hwc/hwc_debugger.cpp \
    hwc/hwc_buffer_allocator.cpp \
    hwc/hwc_buffer_sync_handler.cpp \
    hwc/hwc_color_manager.cpp \
    hwc/blit_engine_c2d.cpp \
    hwc/cpuhint.cpp
include $(BUILD_SHARED_LIBRARY)

LOCAL_PATH := $(DEVICE_LOCAL_PATH)
include $(DEVICE_LOCAL_PATH)/gatekeeper/Android.mk

# Build the Oreo CAF Qualcomm media stack for msm8937. The top-level
# hardware/qcom/media Android.mk does not select msm8998 sources for msm8937
# in this tree, so include the matching CAF variant from the device tree.
QCOM_MEDIA_ROOT := hardware/qcom/media/msm8953
include $(QCOM_MEDIA_ROOT)/mm-core/Android.mk
include $(QCOM_MEDIA_ROOT)/libstagefrighthw/Android.mk
include $(DEVICE_LOCAL_PATH)/qcom-media/common/Android.mk
include $(DEVICE_LOCAL_PATH)/qcom-media/vdec/Android.mk

# Use the device-side venc include so the absent software MPEG4 encoder
# dependency is not emitted, while keeping the hardware AVC VIDC encoder.
include $(DEVICE_LOCAL_PATH)/qcom-media/venc/Android.mk

ifeq ($(BOARD_USES_ADRENO), true)
include $(DEVICE_LOCAL_PATH)/qcom-media/libc2dcolorconvert/Android.mk
endif

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
