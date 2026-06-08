LOCAL_PATH := hardware/qcom/media/msm8953/mm-video-v4l2/vidc/venc
include $(CLEAR_VARS)

# ---------------------------------------------------------------------------------
# 				Common definitons
# ---------------------------------------------------------------------------------

libmm-venc-def := -g -O3 -Dlrintf=_ffix_r
libmm-venc-def += -D__align=__alignx
libmm-venc-def += -D__alignx\(x\)=__attribute__\(\(__aligned__\(x\)\)\)
libmm-venc-def += -DT_ARM
libmm-venc-def += -Dinline=__inline
libmm-venc-def += -D_ANDROID_
libmm-venc-def += -UENABLE_DEBUG_LOW
libmm-venc-def += -UENABLE_DEBUG_HIGH
libmm-venc-def += -DENABLE_DEBUG_ERROR
libmm-venc-def += -UINPUT_BUFFER_LOG
libmm-venc-def += -UOUTPUT_BUFFER_LOG
libmm-venc-def += -USINGLE_ENCODER_INSTANCE
libmm-venc-def += -Werror
libmm-venc-def += -D_ANDROID_ICS_
libmm-venc-def += -D_MSM8974_
libmm-venc-def += -include $(DEVICE_LOCAL_PATH)/qcom-media/vidc_compat.h

TARGETS_THAT_USE_FLAG_MSM8226 := msm8226 msm8916 msm8909
TARGETS_THAT_NEED_SW_VENC_MPEG4 := msm8909
TARGETS_THAT_NEED_SW_VENC_HEVC := msm8992

ifeq ($(TARGET_BOARD_PLATFORM),msm8610)
libmm-venc-def += -DMAX_RES_720P
libmm-venc-def += -D_MSM8610_
else
ifeq ($(TARGET_BOARD_PLATFORM),msm8226)
libmm-venc-def += -DMAX_RES_1080P
else
libmm-venc-def += -DMAX_RES_1080P
libmm-venc-def += -DMAX_RES_1080P_EBI
endif
endif

ifeq ($(TARGET_BOARD_PLATFORM),msm8996)
libmm-venc-def += -D_UBWC_
endif

ifeq ($(call is-board-platform-in-list, $(TARGETS_THAT_USE_FLAG_MSM8226)),true)
libmm-venc-def += -D_MSM8226_
endif

ifeq ($(TARGET_USES_ION),true)
libmm-venc-def += -DUSE_ION
endif

ifeq ($(call is-board-platform-in-list, $(MASTER_SIDE_CP_TARGET_LIST)),true)
libmm-venc-def += -DMASTER_SIDE_CP
endif

# Common Includes
libmm-venc-inc      := $(LOCAL_PATH)/inc
libmm-venc-inc      += $(QCOM_MEDIA_ROOT)/mm-video-v4l2/vidc/common/inc
libmm-venc-inc      += $(QCOM_MEDIA_ROOT)/mm-core/inc
libmm-venc-inc      += $(QCOM_MEDIA_ROOT)/libstagefrighthw
libmm-venc-inc      += $(TARGET_OUT_HEADERS)/qcom/display
libmm-venc-inc      += hardware/qcom/display/msm8996/libqdutils
libmm-venc-inc      += hardware/qcom/display/msm8996/libgralloc
libmm-venc-inc      += hardware/qcom/display/msm8998/include
libmm-venc-inc      += hardware/qcom/display/msm8998/libcopybit
libmm-venc-inc      += $(TARGET_OUT_HEADERS)/adreno
libmm-venc-inc      += frameworks/native/include/media/hardware
libmm-venc-inc      += frameworks/native/include/media/openmax
libmm-venc-inc      += $(QCOM_MEDIA_ROOT)/libc2dcolorconvert
libmm-venc-inc      += $(TARGET_OUT_HEADERS)/libvqzip
libmm-venc-inc      += frameworks/av/include/media/stagefright
ifeq ($(TARGET_COMPILE_WITH_MSM_KERNEL),true)
libmm-venc-inc      += $(TARGET_OUT_INTERMEDIATES)/KERNEL_OBJ/usr/include
endif

# Common Dependencies
ifeq ($(TARGET_COMPILE_WITH_MSM_KERNEL),true)
libmm-venc-add-dep  := $(TARGET_OUT_INTERMEDIATES)/KERNEL_OBJ/usr
endif

# ---------------------------------------------------------------------------------
# 			Make the Shared library (libOmxVenc)
# ---------------------------------------------------------------------------------

include $(CLEAR_VARS)

LOCAL_MODULE                    := libOmxVenc
LOCAL_MODULE_TAGS               := optional
LOCAL_PROPRIETARY_MODULE        := true
LOCAL_CFLAGS                    := $(libmm-venc-def)
LOCAL_C_INCLUDES                := $(libmm-venc-inc)
LOCAL_ADDITIONAL_DEPENDENCIES   := $(libmm-venc-add-dep)

LOCAL_PRELINK_MODULE      := false
LOCAL_SHARED_LIBRARIES    := liblog libutils libbinder libcutils \
                             libc2dcolorconvert libdl libgui
LOCAL_SHARED_LIBRARIES    += libqdMetaData
LOCAL_STATIC_LIBRARIES    := libOmxVidcCommon

LOCAL_SRC_FILES   := src/omx_video_base.cpp
LOCAL_SRC_FILES   += src/omx_video_encoder.cpp
LOCAL_SRC_FILES   += src/video_encoder_device_v4l2.cpp
LOCAL_SRC_FILES   += ../../../../../../../device/samsung/elitelte/qcom-media/venc/get_metadata_compat.cpp

include $(BUILD_SHARED_LIBRARY)

ifeq ($(call is-board-platform-in-list, $(TARGETS_THAT_NEED_SW_VENC_MPEG4)),true)
# ---------------------------------------------------------------------------------
# 			Make the Shared library (libOmxSwVencMpeg4)
# ---------------------------------------------------------------------------------

include $(CLEAR_VARS)

libmm-venc-inc      += $(TARGET_OUT_HEADERS)/mm-video/swvenc

LOCAL_MODULE                    := libOmxSwVencMpeg4

LOCAL_MODULE_TAGS               := optional
LOCAL_PROPRIETARY_MODULE        := true
LOCAL_CFLAGS                    := $(libmm-venc-def)
LOCAL_C_INCLUDES                := $(libmm-venc-inc)
LOCAL_ADDITIONAL_DEPENDENCIES   := $(libmm-venc-add-dep)

LOCAL_PRELINK_MODULE      := false
LOCAL_SHARED_LIBRARIES    := liblog libutils libbinder libcutils \
                             libc2dcolorconvert libdl libgui
LOCAL_SHARED_LIBRARIES    += libMpeg4SwEncoder
LOCAL_STATIC_LIBRARIES    := libOmxVidcCommon

LOCAL_SRC_FILES   := src/omx_video_base.cpp
LOCAL_SRC_FILES   += src/omx_swvenc_mpeg4.cpp

include $(BUILD_SHARED_LIBRARY)
endif

ifeq ($(call is-board-platform-in-list, $(TARGETS_THAT_NEED_SW_VENC_HEVC)),true)
# ---------------------------------------------------------------------------------
#                            Make the Shared library (libOmxSwVenc)
# ---------------------------------------------------------------------------------
include $(CLEAR_VARS)

libmm-venc-inc      += $(TARGET_OUT_HEADERS)/mm-video/swVenc

LOCAL_MODULE                    := libOmxSwVencHevc
LOCAL_MODULE_TAGS               := optional
LOCAL_PROPRIETARY_MODULE        := true
LOCAL_CFLAGS                    := $(libmm-venc-def)
LOCAL_C_INCLUDES                := $(libmm-venc-inc)
LOCAL_ADDITIONAL_DEPENDENCIES   := $(libmm-venc-add-dep)

LOCAL_PRELINK_MODULE      := false
LOCAL_SHARED_LIBRARIES    := liblog libutils libbinder libcutils \
                             libc2dcolorconvert libdl libgui
LOCAL_SHARED_LIBRARIES    += libHevcSwEncoder
LOCAL_STATIC_LIBRARIES    := libOmxVidcCommon

LOCAL_SRC_FILES   := src/omx_video_base.cpp
LOCAL_SRC_FILES   += src/omx_swvenc_hevc.cpp

include $(BUILD_SHARED_LIBRARY)
endif

# ---------------------------------------------------------------------------------
# 					END
# ---------------------------------------------------------------------------------
