MY_PATH := $(call my-dir)/../
LOCAL_PATH := $(call my-dir)/../../../../engine
include $(CLEAR_VARS)

LOCAL_MODULE    := gcube
LOCAL_SRC_FILES := ../platforms/Android/jni/android/ndk-interface.cpp \
                   ApplicationController.cpp \
                   math/Matrix3D.cpp \
                   math/Vector3D.cpp \
                   scene/Camera.cpp \
                   scene/Coords.cpp \
                   scene/Drawable.cpp \
                   scene/Mesh.cpp \
                   scene/Node.cpp \
                   scene/Scene.cpp \
                   scene/SceneTransition.cpp \
                   scene/StandardCamera.cpp \
                   sound/SoundData.cpp \
                   sound/SoundPlayer.cpp \
                   external/math/matrixUtil.c \
                   external/math/vectorUtil.c \
                   external/stb/stb_image.c \
                   external/stb/stb_vorbis.c

LOCAL_CFLAGS    := -Werror -D__GCube_Android__ 
LOCAL_LDLIBS    := -llog -lGLESv2
  
LOCAL_C_INCLUDES:= $(LOCAL_PATH)/ \
                   $(MY_PATH)/OpenAL/ \
                   $(MY_PATH)/OpenAL/include \
                   $(MY_PATH)/OpenAL/OpenAL32/Include \
                   $(LOCAL_PATH)/sound \
                   $(LOCAL_PATH)/scene \
                   $(LOCAL_PATH)/math \
                   $(LOCAL_PATH)/util \
                   $(LOCAL_PATH)/external/math/ \
                   $(LOCAL_PATH)/external/stb/ \
				   $(LOCAL_PATH)/../main/

LOCAL_STATIC_LIBRARIES := openal

include $(BUILD_STATIC_LIBRARY)
