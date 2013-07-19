//
//  Node.h
//  GCube
//
//  Created by Takashi Tsuchiya on 2013/07/18.
//  Copyright (c) 2013年 GClue, Inc. All rights reserved.
//

#ifndef __GCube__Node__
#define __GCube__Node__

#include <vector>

namespace GCube {
	
class Node {
public:
	Node(Node* parent = NULL, const char* name = NULL);
	virtual ~Node();
	
	virtual void update();
	
	Node* getParentNode() const;
	void setParentNode(Node* newParent);
	
	void addChildNode(Node* childNode);
	void removeChildNode(Node* childNode);
	
private:
	Node *parent;
	const char *name;
	std::vector<Node*> children;
	
};
	
}

#endif /* defined(__GCube__Node__) */
