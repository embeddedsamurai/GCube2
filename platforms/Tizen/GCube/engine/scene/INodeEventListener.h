//
//  INodeEventListener.h
//  GCube
//
//  Created by Takashi Tsuchiya on 2013/08/30.
//  Copyright (c) 2013年 GClue, Inc. All rights reserved.
//

#ifndef GCube_INodeEventListener_h
#define GCube_INodeEventListener_h


namespace GCube {

class TouchableNode;

class INodeEventListener {
public:
	virtual~INodeEventListener() {};
	
	virtual void onTouchNode(TouchableNode& node, const TouchEvent &event) {};
};

}


#endif
