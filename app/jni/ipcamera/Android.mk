﻿LOCAL_PATH := $(call my-dir)
LOCAL_PATH := $(call my-dir)

#$(warning $(LOCAL_PATH))

include $(CLEAR_VARS)

###########################
#
# NetVMSSDK_jni library
#
###########################
#注：编辑.a文件时，将.so的依赖性注释

LOCAL_ROOT_PATH := $(LOCAL_PATH)/../../src/main/cpp/ipcamera
LOCAL_LIB_PATH := $(LOCAL_PATH)/../../src/main/cpp/ipcamera/lib

#.a
#include $(CLEAR_VARS)
#LOCAL_MODULE := mXML
#LOCAL_SRC_FILES := $(LOCAL_LIB_PATH)/libmXML.a
#include $(PREBUILT_STATIC_LIBRARY)

#include $(CLEAR_VARS)
#LOCAL_MODULE := NDRender
#LOCAL_SRC_FILES := $(LOCAL_LIB_PATH)/libNDRender.a
#include $(PREBUILT_STATIC_LIBRARY)

#include $(CLEAR_VARS)
#LOCAL_MODULE := MP4
#LOCAL_SRC_FILES := $(LOCAL_LIB_PATH)/libMP4.a
#include $(PREBUILT_STATIC_LIBRARY)

#include $(CLEAR_VARS)
#LOCAL_MODULE := RM_Module
#LOCAL_SRC_FILES := $(LOCAL_LIB_PATH)/libRM_Module.a
#include $(PREBUILT_STATIC_LIBRARY)

#include $(CLEAR_VARS)
#LOCAL_MODULE := NDPlayer
#LOCAL_SRC_FILES := $(LOCAL_LIB_PATH)/libNDPlayer.a
#include $(PREBUILT_STATIC_LIBRARY)

#include $(CLEAR_VARS)
#LOCAL_MODULE := Curl
#LOCAL_SRC_FILES := $(LOCAL_LIB_PATH)/libCurl.a
#include $(PREBUILT_STATIC_LIBRARY)

#.so
include $(CLEAR_VARS)
LOCAL_MODULE := mXML
LOCAL_SRC_FILES := $(LOCAL_LIB_PATH)/libmXML.so
include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := NDRender
LOCAL_SRC_FILES := $(LOCAL_LIB_PATH)/libNDRender.so
include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := MP4
LOCAL_SRC_FILES := $(LOCAL_LIB_PATH)/libMP4.so
include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := RM_Module
LOCAL_SRC_FILES := $(LOCAL_LIB_PATH)/libRM_Module.so
include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := NDPlayer
LOCAL_SRC_FILES := $(LOCAL_LIB_PATH)/libNDPlayer.so
include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := Curl
LOCAL_SRC_FILES := $(LOCAL_LIB_PATH)/libCurl.so
include $(PREBUILT_SHARED_LIBRARY) 

include $(CLEAR_VARS)
LOCAL_MODULE := Discovery
LOCAL_SRC_FILES := $(LOCAL_LIB_PATH)/libDiscovery.so
include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := DspVideoMjpeg
LOCAL_SRC_FILES := $(LOCAL_LIB_PATH)/libdspvideomjpeg.so
include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := Stun
LOCAL_SRC_FILES := $(LOCAL_LIB_PATH)/libstun.a
include $(PREBUILT_STATIC_LIBRARY) 

include $(CLEAR_VARS)
LOCAL_MODULE := CloudHttpCurl
LOCAL_SRC_FILES := $(LOCAL_LIB_PATH)/libcloudhttpcurl.a
include $(PREBUILT_STATIC_LIBRARY) 

include $(CLEAR_VARS)
LOCAL_MODULE := T2U
LOCAL_SRC_FILES := $(LOCAL_LIB_PATH)/libt2u.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := CloudClient
LOCAL_SRC_FILES := $(LOCAL_LIB_PATH)/libcloudclient.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := Cloud
LOCAL_SRC_FILES := $(LOCAL_LIB_PATH)/libcloud.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := Event
LOCAL_SRC_FILES := $(LOCAL_LIB_PATH)/libevent.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE :=libNetDEVSDK
LOCAL_SRC_FILES := $(LOCAL_LIB_PATH)/libNetDEVSDK.so
include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)

LOCAL_MODULE    := ipcamera_sdk

LOCAL_C_INCLUDES = -I $(LOCAL_ROOT_PATH)/
LOCAL_C_INCLUDES += -I $(LOCAL_ROOT_PATH)/include/
LOCAL_C_INCLUDES += -I $(LOCAL_ROOT_PATH)/../interface/
LOCAL_C_INCLUDES += -I $(LOCAL_ROOT_PATH)/../util/
LOCAL_C_INCLUDES += -I $(LOCAL_ROOT_PATH)/../util/alox/

#源文件
LOCAL_SRC_FILES += $(wildcard $(LOCAL_ROOT_PATH)/../util/ALoxHelper.cpp)

LOCAL_SRC_FILES += $(wildcard $(LOCAL_ROOT_PATH)/IPCamera.cpp)
LOCAL_SRC_FILES += $(wildcard $(LOCAL_ROOT_PATH)/IPCameraJNI.cpp)

LOCAL_LDLIBS :=  -fPIC -shared -llog -lGLESv2 -lOpenSLES -landroid -lc -lz

#.a

#LOCAL_STATIC_LIBRARIES := mXML 
#LOCAL_STATIC_LIBRARIES += NDRender 
#LOCAL_STATIC_LIBRARIES += MP4
#LOCAL_STATIC_LIBRARIES += RM_Module
#LOCAL_STATIC_LIBRARIES += NDPlayer
#LOCAL_STATIC_LIBRARIES += Curl
#include $(BUILD_STATIC_LIBRARY)

#.so
LOCAL_SHARED_LIBRARIES := mXML
LOCAL_SHARED_LIBRARIES += NDRender
LOCAL_SHARED_LIBRARIES += MP4
LOCAL_SHARED_LIBRARIES += RM_Module
LOCAL_SHARED_LIBRARIES += NDPlayer
LOCAL_SHARED_LIBRARIES += Curl
LOCAL_SHARED_LIBRARIES += Discovery
LOCAL_SHARED_LIBRARIES += DspVideoMjpeg
LOCAL_SHARED_LIBRARIES += libNetDEVSDK

LOCAL_WHOLE_STATIC_LIBRARIES += Event
LOCAL_WHOLE_STATIC_LIBRARIES += T2U
LOCAL_WHOLE_STATIC_LIBRARIES += Stun
LOCAL_WHOLE_STATIC_LIBRARIES += CloudHttpCurl
LOCAL_WHOLE_STATIC_LIBRARIES += Cloud
LOCAL_WHOLE_STATIC_LIBRARIES += CloudClient
LOCAL_CFLAGS := -fPIC -fpermissive -D__STDC_CONSTANT_MACROS -DBOOST_NO_STD_WSTRING -DNETVMS_SDK_DLL -D__TENNIS_IPCAMERA_MODULE__ -D__ANDROID_LOG_CONSOLE__
LOCAL_CPPFLAGS := -std=c++11 -frtti -fexceptions
include $(BUILD_SHARED_LIBRARY)