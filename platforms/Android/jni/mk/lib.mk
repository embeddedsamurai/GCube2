MY_PATH := $(call my-dir)/../
LOCAL_PATH := $(call my-dir)/../../../../engine
include $(CLEAR_VARS)

LOCAL_MODULE    := gcube
LOCAL_SRC_FILES := ../platforms/Android/jni/android/ndk-interface.cpp \
                   ApplicationController.cpp \
                   external/math/matrixUtil.c \
                   external/math/vectorUtil.c \
                   external/stb/stb_image.c \
                   external/stb/stb_vorbis.c \
                   math/Matrix3D.cpp \
                   math/Vector3D.cpp \
                   scene/Camera.cpp \
                   scene/Coords.cpp \
                   scene/Figure.cpp \
                   scene/Light.cpp \
                   scene/Mesh.cpp \
                   scene/Node.cpp \
                   scene/PrimitiveObject.cpp \
                   scene/Scene.cpp \
                   scene/SceneTransition.cpp \
                   scene/StandardCamera.cpp \
                   scene/Texture.cpp \
                   scene/TextureData.cpp \
                   scene/Vbo.cpp \
                   scene/Window.cpp \
                   shader/ColorShader.cpp \
                   shader/FlatShader.cpp \
                   shader/Shader.cpp \
                   shader/TexShader.cpp \
                   sound/SoundData.cpp \
                   sound/SoundPlayer.cpp
                   

LOCAL_CFLAGS    := -Werror -D__GCube_Android__ 
LOCAL_LDLIBS    := -llog -lGLESv2
  
LOCAL_C_INCLUDES:= $(LOCAL_PATH)/ \
                   $(MY_PATH)/OpenAL/ \
                   $(MY_PATH)/OpenAL/include \
                   $(MY_PATH)/OpenAL/OpenAL32/Include \
                   $(LOCAL_PATH)/external/math/ \
                   $(LOCAL_PATH)/external/stb/ \
                   $(LOCAL_PATH)/math \
                   $(LOCAL_PATH)/scene \
                   $(LOCAL_PATH)/shader \
                   $(LOCAL_PATH)/sound \
                   $(LOCAL_PATH)/util \
				   $(LOCAL_PATH)/../main/

LOCAL_STATIC_LIBRARIES := openal

include $(BUILD_STATIC_LIBRARY)
