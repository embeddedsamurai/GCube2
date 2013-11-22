/*
 * The MIT License (MIT)
 * Copyright (c) 2013 GClue, inc.
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#ifndef GCube_GCDefines_h
#define GCube_GCDefines_h

#include <stdlib.h>
#include <stddef.h>
#include <math.h>
#include <string>
#include <vector>
#include <map>
#include <memory>

// Android
#ifdef __GCube_Android__
# include <GLES2/gl2.h>
# include <GLES2/gl2ext.h>
# include <AL/al.h>
# include <AL/alc.h>
#define DEF_SHARED_PTR(name) typedef std::shared_ptr<name> name##_ptr;typedef std::weak_ptr<name> name##_wkptr
#define PTR_CAST(name, val) std::dynamic_pointer_cast<name>(val)
#endif

// iOS
#ifdef __GCube_iOS__
# include <OpenGLES/ES2/gl.h>
# include <OpenGLES/ES2/glext.h>
# include <OpenAL/al.h>
# include <OpenAL/alc.h>
#define DEF_SHARED_PTR(name) typedef std::shared_ptr<name> name##_ptr;typedef std::weak_ptr<name> name##_wkptr
#define PTR_CAST(name, val) std::dynamic_pointer_cast<name>(val)
#endif

// Tizen
#ifdef __GCube_Tizen__
# include <gl2.h>
# include <gl2ext.h>
# include <AL/al.h>
# include <AL/alc.h>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#define DEF_SHARED_PTR(name) typedef boost::shared_ptr< name > name##_ptr;typedef boost::weak_ptr< name > name##_wkptr
#define PTR_CAST(name, val) boost::dynamic_pointer_cast<name>(val)
#endif


#define DEGREES_TO_RADIANS(angle) ((angle) / 180.0 * M_PI)
#define RADIANS_TO_DEGREES(radians) ((radians) * (180.0 / M_PI))
#define CONST_STR(name, str) static const char name[]=#str

namespace GCube {

// クラス
class Main;
class Scene;
class SceneTransition;

// デバイスの向き
typedef enum DeviceOrientation
{
	DeviceOrientationUnknown,
	DeviceOrientationPortrait,            // Device oriented vertically, home button on the bottom
	DeviceOrientationPortraitUpsideDown,  // Device oriented vertically, home button on the top
	DeviceOrientationLandscapeLeft,       // Device oriented horizontally, home button on the right
	DeviceOrientationLandscapeRight,      // Device oriented horizontally, home button on the left
	DeviceOrientationFaceUp,              // Device oriented flat, face up
	DeviceOrientationFaceDown             // Device oriented flat, face down
}
DeviceOrientation;

// タッチイベント
typedef enum TouchAction
{
	TouchActionDown,
	TouchActionUp,
	TouchActionMove,
	TouchActionCancel,
}
TouchAction;

// ストレージタイプ
typedef enum StorageType
{
	StorageTypeDocument,
	StorageTypeCache,
}
StorageType;

typedef struct Pointf {
	float x;
	float y;
	Pointf(float x=0, float y=0) : x(x), y(y) {};
} Pointf;

typedef struct Sizef {
	float width;
	float height;
	Sizef(float width=0, float height=0) : width(width), height(height) {};
} Sizef;

typedef struct Point3f {
	float x;
	float y;
	float z;
	Point3f(float x=0, float y=0, float z=0) : x(x), y(y), z(z) {};
} Point3f;

typedef struct Colorf {
	float r;
	float g;
	float b;
	float a;
	Colorf(float r=0, float g=0, float b=0, float a=1): r(r), g(g), b(b), a(a) {};
	bool equals(const Colorf &color) {
		return (r == color.r && g == color.g && b == color.b);
	}
} Colorf;

typedef struct Rectf {
	Pointf location;
	Sizef size;
	
	Rectf(float x=0, float y=0, float width=0, float height=0) {
		location.x = x;
		location.y = y;
		size.width = width;
		size.height = height;
	};
	
} Rectf;

typedef struct TouchEvent {
	TouchAction action;
	Pointf pos;
	long id;
	long time;
	TouchEvent(TouchAction action, float x, float y, long id, long time) : action(action), pos(Pointf(x, y)), id(id), time(time) {};
} TouchEvent;

typedef enum {
	AttribTypeVertex,
	AttribTypeNormal,
	AttribTypeColor,
	AttribTypeUV,
	AttribTypeUV2,
	AttribTypeUV3,
	AttribTypeUV4,
	NUM_ATTRIB_TYPE
} AttribType;

}


/////////////////////////////////////////////////////////////////////////
#pragma mark - API glue

// 言語コードを取得
std::string GCGetLanguage();
// リソースを取得
void GCGetResourceData(const char *fileName, std::vector<char>& outData);
// ストレージパスを取得
std::string GCGetStoragePath(GCube::StorageType type);

// ユーザーイベントを送信
int GCSendUserEvent(int type, int param1, long long param2, float param3, double param4, const char *param5);



#endif
