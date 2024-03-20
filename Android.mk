LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE_TAGS := optional


LOCAL_SRC_FILES += src/tc_api_color_adjustment.c \
                   src/tc_color_3d_tables.c \
		   src/tc_color_hsv.c \
		   src/tc_color_log.c \
		   src/tc_color_temperature.c \
		   src/tc_color_util.c

LOCAL_C_INCLUDES += $(LOCAL_PATH)/include

LOCAL_MODULE := libtc-color-adjustment

include $(BUILD_SHARED_LIBRARY)

