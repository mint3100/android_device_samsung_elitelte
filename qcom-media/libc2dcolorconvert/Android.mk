LOCAL_PATH := hardware/qcom/media/msm8953/libc2dcolorconvert

include $(CLEAR_VARS)

LOCAL_SRC_FILES := \
        C2DColorConverter.cpp

LOCAL_C_INCLUDES := \
    $(TARGET_OUT_HEADERS)/qcom/display
LOCAL_C_INCLUDES += hardware/qcom/display/msm8996/libgralloc
LOCAL_C_INCLUDES += hardware/qcom/display/msm8998/include
LOCAL_C_INCLUDES += hardware/qcom/display/msm8998/libcopybit

ifeq ($(TARGET_COMPILE_WITH_MSM_KERNEL),true)
LOCAL_C_INCLUDES += $(TARGET_OUT_INTERMEDIATES)/KERNEL_OBJ/usr/include
endif

LOCAL_SHARED_LIBRARIES := liblog libdl

LOCAL_MODULE_TAGS := optional

LOCAL_MODULE := libc2dcolorconvert

ifeq ($(TARGET_COMPILE_WITH_MSM_KERNEL),true)
LOCAL_ADDITIONAL_DEPENDENCIES := $(TARGET_OUT_INTERMEDIATES)/KERNEL_OBJ/usr
endif

LOCAL_PROPRIETARY_MODULE := true

include $(BUILD_SHARED_LIBRARY)
