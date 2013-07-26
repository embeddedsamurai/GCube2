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

#include "Node.h"
//#include "../GCDefines.h"
//#include "Coords.h"
//#include "Camera.h"
#include "Drawable.h"

using namespace GCube;

// コンストラクタ
Node::Node(Node *parent, const char *name) : name(name) {
	this->parent = parent;
}

// デストラクタ
Node::~Node() {
	parent = NULL;
	children.clear();
}

// 処理更新
void Node::updateProcess(float dt) {
	// 子
	if(!children.empty()) {
		for(size_t i = 0; i < children.size(); ++i) {
			if(NULL != children[i]) {
				children[i]->updateProcess(dt);
			}
		}
	}
}

// 描画
void Node::drawProcess() {
	// 軸表示
	drawAxis();
	// 描画
	Drawable *drawable = dynamic_cast<Drawable*>(this);
	if (drawable && drawable->isVisible) {
		drawable->draw();
	}
	// 子
	if(!children.empty()) {
		for(size_t i = 0; i < children.size(); ++i) {
			if(NULL != children[i]) {
				children[i]->drawProcess();
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
