LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

###########################
#
# tennisdrill library
#
###########################
#注：编辑.a文件时，将.so的依赖性注释

LOCAL_ROOT_PATH := $(LOCAL_PATH)/../../src/main/cpp/tennisdrill

LOCAL_MODULE    := tennisdrill

LOCAL_C_INCLUDES = -I $(LOCAL_ROOT_PATH)/
LOCAL_C_INCLUDES += -I $(LOCAL_ROOT_PATH)/../interface/
LOCAL_C_INCLUDES += -I $(LOCAL_ROOT_PATH)/../util/
LOCAL_C_INCLUDES += -I $(LOCAL_ROOT_PATH)/../util/alox/
LOCAL_C_INCLUDES += -I $(LOCAL_ROOT_PATH)/../util/libevent/src/include/

#源文件
LOCAL_SRC_FILES += $(wildcard $(LOCAL_ROOT_PATH)/../util/ALoxHelper.cpp)
LOCAL_SRC_FILES += $(wildcard $(LOCAL_ROOT_PATH)/../util/TennisBaseJNI.cpp)
LOCAL_SRC_FILES += $(wildcard $(LOCAL_ROOT_PATH)/../util/TennisBaseImpl.cpp)

LOCAL_SRC_FILES += $(wildcard $(LOCAL_ROOT_PATH)/TennisDrillJNI.cpp)
LOCAL_SRC_FILES += $(wildcard $(LOCAL_ROOT_PATH)/TennisDrillJNIImpl.cpp)
LOCAL_SRC_FILES += $(wildcard $(LOCAL_ROOT_PATH)/TennisDrillImpl.cpp)

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

LOCAL_CFLAGS := -fPIC -D__TENNIS_DRILL_MODULE__ -D__TENNIS_DRILL_TEST__ -D__ANDROID_LOG_CONSOLE__
LOCAL_CPPFLAGS := -std=c++11 -frtti -fexceptions

include $(BUILD_SHARED_LIBRARY)