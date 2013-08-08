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

#ifndef __GCube__Scene__
#define __GCube__Scene__

#include "GCDefines.h"
#include "Node.h"
#include "Camera.h"
#include "SceneTransition.h"
#include "Window.h"

namespace GCube {

class Scene {
public:
	Scene(const char* name = NULL);
	virtual ~Scene();
	
	virtual void onUpdate(float dt);
	virtual void onDraw(Window &window);
	
//	virtual void changeCamera(const Camera_ptr &nextCamera, SceneTransition *transition=NULL);

public:
	Node rootNode;
	
private:
//	Camera_ptr activeCamera;
	const char *name;
	
};

DEF_SHARED_PTR(Scene);

}

#endif /* defined(__GCube__Scene__) */
