LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_SRC_FILES := lights.c
LOCAL_MODULE := lights.$(TARGET_BOARD_PLATFORM)
LOCAL_MODULE_RELATIVE_PATH := hw
LOCAL_MODULE_TAGS := optional
LOCAL_PROPRIETARY_MODULE := true
LOCAL_SHARED_LIBRARIES := liblog
LOCAL_CFLAGS := -Wall -Wextra -Wno-unused-parameter -DLOG_TAG=\"ElitelteLights\"
LOCAL_CLANG := true
include $(BUILD_SHARED_LIBRARY)
