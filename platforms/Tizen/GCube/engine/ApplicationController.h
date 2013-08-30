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

#ifndef __GCube__ApplicationController__
#define __GCube__ApplicationController__

#include "GCDefines.h"
#include "IApplicationEventListener.h"
#include "scene/Scene.h"
#include "scene/Window.h"
#include "scene/TextureCache.h"
#include "shader/ShaderManager.h"

namespace GCube {

class ApplicationController : public IApplicationEventListener {
private:
	Main *main;
	Scene_ptr activeScene;
	Sizef screenSize;
	float aspect;
	
	GLuint hitTestFramebuffer;
	GLuint hitTestRenderbuffer;
	
public:
	Colorf bgColor;
	std::vector<Window_ptr> windowArray;
	TextureCache textureCache;
	ShaderManager shaderManager;
	
private:
	ApplicationController();
	
public:
	virtual~ApplicationController();
	
	static ApplicationController* SharedInstance();
	static void DestroyInstance();
	
	// Scene //
	virtual void changeScene(const Scene_ptr &nextScene, SceneTransition *transition=NULL);
	virtual Scene_ptr getActiveScene() {return activeScene;};
	
	// Utils //
	virtual float getVersion();
	virtual std::string getLanguage();
	virtual void getResource(const char *fileName, std::vector<char>& outData);
	virtual std::string getStoragePath(StorageType type);
	
	virtual int sendUserEvent(int type, int param1=0, long long param2=0, float param3=0, double param4=0, const char *param5=NULL);
	
	// IApplicationEventListener //
	virtual void onInit();
	virtual void onTerminate();
	virtual void onPause();
	virtual void onResume();
	virtual void onContextChanged();
	virtual void onSizeChanged(float width, float height, DeviceOrientation orientation);
	virtual void onLowMemory();
	virtual void onUpdate(float dt);
	virtual void onDraw();
	virtual void onTouch(TouchAction action, float x, float y, long id, long time);
	virtual void onOrientationChanged(float yaw, float pitch, float roll);
	virtual int onUserEvent(int type, int param1=0, long long param2=0, float param3=0, double param4=0, const char *param5=NULL);
	virtual void onDebugCommand(const char *command, int param);
};
	
}

#endif /* defined(__GCube__ApplicationController__) */
