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
#include "DrawableNode.h"
#include "Figure.h"
#include "TouchableNode.h"
#include "../util/Log.h"
#include <string.h>

using namespace GCube;

// コンストラクタ
Node::Node(const char *name) : tag(0), name(name), parent(NULL), type(0) {
}

// デストラクタ
Node::~Node() {
	parent = NULL;
	children.clear();
}


// 処理更新
void Node::updateProcess(float dt, const Matrix3D &matrix) {
	// グローバル座標を計算
	globalMatrix = Matrix3D(matrix);
	globalMatrix.multiply(&this->transform);
	// 子
	if(!children.empty()) {
		for(size_t i = 0; i < children.size(); ++i) {
			if(NULL != children[i]) {
				children[i]->updateProcess(dt, globalMatrix);
			}
		}
	}
}

// 描画
void Node::drawProcess(const Window &window, bool hitTest) {
	// 軸表示
	drawAxis();
	// 描画
	DrawableNode *drawable = dynamic_cast<DrawableNode*>(this);
	if (drawable && drawable->isVisible) {
		if (hitTest) {
			drawable->draw(window, DrawTypeHitTest);
		} else {
			drawable->draw(window);
		}
	}
	// 子
	if(!children.empty()) {
		for(size_t i = 0; i < children.size(); ++i) {
			if(NULL != children[i]) {
				children[i]->drawProcess(window, hitTest);
			}
		}
	}
}

// 処理
void Node::hitTestProcess(const TouchEvent &event, const Colorf &color) {
	// Hitテスト
	TouchableNode *node = dynamic_cast<TouchableNode*>(this);
	if (node && node->isEnable) {
		node->hitTest(event, color);
	}
	// 子
	if(!children.empty()) {
		for(size_t i = 0; i < children.size(); ++i) {
			if(NULL != children[i]) {
				children[i]->hitTestProcess(event, color);
			}
		}
	}
}

// 親取得
Node *Node::getParentNode() const {
	return(parent);
}

// 親設定
void Node::setParentNode(Node *newParent) {
	if(NULL != parent) {
		Node_ptr node(this);
		parent->removeChildNode(node);
	}
	parent = newParent;
}

// 子追加
void Node::addChildNode(const Node_ptr &childNode) {
	if(NULL != childNode) {
		childNode->setParentNode(this);
		children.push_back(childNode);
	}
}

// 子削除
void Node::removeChildNode(const Node_ptr &childNode) {
	if(NULL != childNode && !children.empty()) {
		for(size_t i = 0; i < children.size(); ++i) {
			if(children[i].get() == childNode.get()) {
				children.erase(children.begin() + i);
				break;
			}
		}
	}
}

// 名前指定でノード検索
Node* Node::findChildNodeByName(const char *searchName) {
	// TODO: 全件検索なので重い。。。
	if(0 == strcmp(this->name, searchName)) {
		return this;
	}
	if(!children.empty()) {
		for(size_t i = 0; i < children.size(); ++i) {
			Node *node = children[i]->findChildNodeByName(searchName);
			if(node) {
				return node;
			}
		}
	}
	return NULL;
}

// 名前指定でノード検索
std::vector<Node*> Node::findChildNodeByType(int type) {
	// TODO: 全件検索なので重い。。。
	std::vector<Node*> nodes;
	if(this->type == type) {
		nodes.push_back(this);
	}
	if(!children.empty()) {
		for(size_t i = 0; i < children.size(); ++i) {
			std::vector<Node*> vec = children[i]->findChildNodeByType(type);
			nodes.insert(nodes.end(), vec.begin(), vec.end());
		}
	}
	return nodes;
}

