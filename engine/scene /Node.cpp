//
//  Node.cpp
//  GCube
//
//  Created by Takashi Tsuchiya on 2013/07/18.
//  Copyright (c) 2013年 GClue, Inc. All rights reserved.
//

#include "Node.h"


namespace GCube {
	
// コンストラクタ
Node::Node(Node *parent, const char *name) : name(name) {
	this->parent = parent;
}

// デストラクタ
Node::~Node() {
	parent = NULL;
	children.clear();
}

// update
void Node::update() {
	if(!children.empty()) {
		for(size_t i = 0; i < children.size(); ++i) {
			if(NULL != children[i]) {
				children[i]->update();
			}
		}
	}
}

// 親取得
Node* Node::getParentNode() const {
	return(parent);
}

// 親設定
void Node::setParentNode(Node* newParent) {
	if(NULL != parent) {
		parent->removeChildNode(this);
	}
	parent = newParent;
}

// 子追加
void Node::addChildNode(Node *childNode) {
	if(NULL != childNode) {
		childNode->setParentNode(this);
		children.push_back(childNode);
	}
}

// 子削除
void Node::removeChildNode(Node *childNode) {
	if(NULL != childNode && !children.empty()) {
		for(size_t i = 0; i < children.size(); ++i) {
			if(children[i] == childNode) {
				children.erase(children.begin() + i);
				break;
			}
		}
	}
}

}
