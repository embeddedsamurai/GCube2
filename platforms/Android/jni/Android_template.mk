MY_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_PATH := $(MY_PATH)
include $(CLEAR_VARS)
LOCAL_MODULE := gcube
LOCAL_SRC_FILES := libs/libgcube.a
include $(PREBUILT_STATIC_LIBRARY)
LOCAL_MODULE := openal
LOCAL_SRC_FILES := libs/libopenal.a
include $(PREBUILT_STATIC_LIBRARY)

LOCAL_PATH := $(MY_PATH)/../../..
include $(CLEAR_VARS)

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
                   $(MY_PATH)/Headers/ \
                   $(MY_PATH)/Headers/external/math/ \
                   $(MY_PATH)/Headers/external/stb/ \
                   $(MY_PATH)/Headers/math \
                   $(MY_PATH)/Headers/scene \
                   $(MY_PATH)/Headers/shader \
                   $(MY_PATH)/Headers/sound \
                   $(MY_PATH)/Headers/util \
                   $(MAIN_PATH)
                   

include $(BUILD_SHARED_LIBRARY)

