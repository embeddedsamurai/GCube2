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

#ifndef __GCube__Node__
#define __GCube__Node__

#include "GCDefines.h"
#include "Coords.h"

namespace GCube {

class Window;
class Node;
DEF_SHARED_PTR(Node);

class Node : public Coords {
public:
	Node(const char* name = NULL);
	virtual ~Node();
	
	virtual void updateProcess(float dt, const Matrix3D &matrix);
	virtual void drawProcess(const Window &window, bool hitTest=false);
	virtual void hitTestProcess(const TouchEvent &event, const Colorf &color);
	virtual void touchProcess(const TouchEvent &event);
	
	Node *getParentNode() const;
	
	void addChildNode(const Node_ptr &childNode);
	void removeChildNode(const Node_ptr &childNode);
	
	Node *findChildNodeByName(const char *searchName);
	std::vector<Node*> findChildNodeByType(int type);


public:
	int tag;
	const char *name;

	
protected:
	void setParentNode(Node *newParent);
	Node *parent;
	std::vector<Node_ptr> children;
	int type;
};

}

#endif /* defined(__GCube__Node__) */
