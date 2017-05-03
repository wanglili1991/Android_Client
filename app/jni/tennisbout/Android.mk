LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

###########################
#
# tennisbout library
#
###########################
#注：编辑.a文件时，将.so的依赖性注释

LOCAL_ROOT_PATH := $(LOCAL_PATH)/../../src/main/cpp/tennisbout

LOCAL_MODULE    := tennisbout

LOCAL_C_INCLUDES = -I $(LOCAL_ROOT_PATH)/
LOCAL_C_INCLUDES += -I $(LOCAL_ROOT_PATH)/../interface/
LOCAL_C_INCLUDES += -I $(LOCAL_ROOT_PATH)/../util/
LOCAL_C_INCLUDES += -I $(LOCAL_ROOT_PATH)/../util/alox/
LOCAL_C_INCLUDES += -I $(LOCAL_ROOT_PATH)/../util/libevent/src/include/

#源文件
LOCAL_SRC_FILES += $(wildcard $(LOCAL_ROOT_PATH)/../util/ALoxHelper.cpp)
LOCAL_SRC_FILES += $(wildcard $(LOCAL_ROOT_PATH)/../util/TennisBaseJNI.cpp)
LOCAL_SRC_FILES += $(wildcard $(LOCAL_ROOT_PATH)/../util/TennisBaseImpl.cpp)

LOCAL_SRC_FILES += $(wildcard $(LOCAL_ROOT_PATH)/TennisBoutJNI.cpp)
LOCAL_SRC_FILES += $(wildcard $(LOCAL_ROOT_PATH)/TennisBoutJNIImpl.cpp)
LOCAL_SRC_FILES += $(wildcard $(LOCAL_ROOT_PATH)/TennisBoutImpl.cpp)

LOCAL_LDLIBS :=  -fPIC -shared -llog

#.a

LOCAL_STATIC_LIBRARIES := event
LOCAL_STATIC_LIBRARIES += event_core
LOCAL_STATIC_LIBRARIES += event_extra
LOCAL_STATIC_LIBRARIES += event_pthreads
#include $(BUILD_STATIC_LIBRARY)

#.so
#LOCAL_SHARED_LIBRARIES := event
#LOCAL_SHARED_LIBRARIES += event_core
#LOCAL_SHARED_LIBRARIES += event_extra
#LOCAL_SHARED_LIBRARIES += event_pthreads

LOCAL_CFLAGS := -fPIC -D__TENNIS_BOUT_MODULE__ -D__ANDROID_LOG_CONSOLE__
LOCAL_CPPFLAGS := -std=c++11 -frtti -fexceptions

#可以限定在当前mk中有效 LOCAL_CPPFLAGS 不可以
#LOCAL_CPP_FEATURES := rtti features

include $(BUILD_SHARED_LIBRARY)
