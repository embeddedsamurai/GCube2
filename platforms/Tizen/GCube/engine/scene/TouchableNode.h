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

#ifndef __GCube__TouchableNode__
#define __GCube__TouchableNode__

#include "DrawableNode.h"
#include "INodeEventListener.h"
#include "../shader/HitTestShader.h"
#include "../shader/ShaderManager.h"

namespace GCube {

class TouchableNode : public DrawableNode {
public:
	TouchableNode(const char* name = NULL) : DrawableNode(name), isTouchable(false), touchEventListener(NULL) {
		testShader = ShaderManager::GetShader(ShaderTypeHitTest);
	};
	virtual ~TouchableNode() {};
	
	virtual void hitTest(const TouchEvent &event, const Colorf &color) {
		if (touchEventListener && isTouchable && testColor.equals(color)) {
			isActive = true;
			touchEventListener->onTouchNode(*this, event);
		}
	};
	
	virtual void touch(const TouchEvent &event) {
		if (touchEventListener && isTouchable && isActive) {
			touchEventListener->onTouchNode(*this, event);
		}
		if (event.action == TouchActionUp ||
			event.action == TouchActionCancel) {
			isActive = false;
		}
	};
	
public:
	bool isTouchable;
	Colorf testColor;
	INodeEventListener *touchEventListener;

protected:
	Shader_ptr testShader;
	bool isActive;
	
};
}

#endif /* defined(__GCube__TouchableNode__) */
