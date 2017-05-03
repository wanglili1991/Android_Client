LOCAL_PATH := $(call my-dir)

ROOT_LOCAL_PATH := $(LOCAL_PATH)

include $(ROOT_LOCAL_PATH)/util/libevent/Android.mk
include $(ROOT_LOCAL_PATH)/tennisbout/Android.mk
include $(ROOT_LOCAL_PATH)/tennisdrill/Android.mk
include $(ROOT_LOCAL_PATH)/ipcamera/Android.mk