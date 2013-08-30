//
//  TouchableNode.h
//  GCube
//
//  Created by Takashi Tsuchiya on 2013/08/29.
//  Copyright (c) 2013年 GClue, Inc. All rights reserved.
//

#ifndef __GCube__TouchableNode__
#define __GCube__TouchableNode__

#include "DrawableNode.h"
#include "INodeEventListener.h"
#include "../shader/HitTestShader.h"
#include "../shader/ShaderManager.h"

namespace GCube {

class TouchableNode : public DrawableNode {
public:
	TouchableNode(const char* name = NULL) : DrawableNode(name), isEnable(true), touchEventListener(NULL) {
		testShader = ShaderManager::GetShader(ShaderTypeHitTest);
	};
	virtual ~TouchableNode() {};
	
	virtual void hitTest(const TouchEvent &event, const Colorf &color) {
		if (touchEventListener && testColor.equals(color)) {
			touchEventListener->onTouchNode(*this, event);
		}
	};
	
public:
	bool isEnable;
	Colorf testColor;
	INodeEventListener *touchEventListener;

protected:
	Shader_ptr testShader;
	
};
}

#endif /* defined(__GCube__TouchableNode__) */
