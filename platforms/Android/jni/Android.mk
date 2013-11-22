MY_PATH := $(call my-dir)
include $(CLEAR_VARS)

#include $(MY_PATH)/mk/openal.mk
#include $(CLEAR_VARS)

ifndef BUILD_LIB
	include $(MY_PATH)/mk/lib.mk
else
	LOCAL_PATH := $(MY_PATH)
	include $(CLEAR_VARS)
	LOCAL_MODULE := gcube
	LOCAL_SRC_FILES := libs/libgcube.a
	include $(PREBUILT_STATIC_LIBRARY)
endif
LOCAL_PATH := $(MY_PATH)
include $(CLEAR_VARS)
LOCAL_MODULE := openal
LOCAL_SRC_FILES := libs/libopenal.a
include $(PREBUILT_STATIC_LIBRARY)

LOCAL_PATH := $(MY_PATH)/../../..
include $(CLEAR_VARS)

ENGINE_PATH := $(LOCAL_PATH)/engine
MAIN_PATH := $(LOCAL_PATH)/main

APP_MODULES     := gcube_app
LOCAL_MODULE    := gcube_app
LOCAL_CFLAGS    := -Werror -D__GCube_Android__ 

LOCAL_LDLIBS    := -llog -lGLESv2 -lz -landroid
LOCAL_STATIC_LIBRARIES := gcube openal 

LOCAL_SRC_FILES := main/Main.cpp \
                   main/FirstSceneController.cpp \
                   main/SecondSceneController.cpp
                   
LOCAL_C_INCLUDES:= $(MY_PATH)/android/ \
                   $(MY_PATH)/OpenAL/ \
                   $(MY_PATH)/OpenAL/include \
                   $(MY_PATH)/OpenAL/OpenAL32/Include \
                   $(ENGINE_PATH)/ \
                   $(ENGINE_PATH)/external/math/ \
                   $(ENGINE_PATH)/external/stb/ \
                   $(ENGINE_PATH)/math \
                   $(ENGINE_PATH)/scene \
                   $(ENGINE_PATH)/shader \
                   $(ENGINE_PATH)/sound \
                   $(ENGINE_PATH)/util \
                   $(ENGINE_PATH)/util/loader \
                   $(MAIN_PATH)
                   

include $(BUILD_SHARED_LIBRARY)

