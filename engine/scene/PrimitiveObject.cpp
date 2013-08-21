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

#include "PrimitiveObject.h"

#include "Mesh.h"

using namespace GCube;

// プレートのMeshクラスを作成します.
Mesh_ptr PrimitiveObject::createPlate(Sizef size) {
	float x = size.width / 2.0;
	float y = size.height / 2.0;
	
	const GLfloat posArray[] = {
		-x, -y, 0.0f,
		 x, -y, 0.0f,
		 x,  y, 0.0f,
		-x,  y, 0.0f
	};
	
	const GLfloat texcoordArray[] = {
		0.0f,  0.0f,
		1.0f,  0.0f,
		1.0f,  1.0f,
		0.0f,  1.0f,
	};
	
	const GLfloat normalArray[] = {
		0.0f, 0.0f, 1.0,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
	};
	
	const GLushort elementArray[] = {
		0, 1, 2,
		0, 2, 3
	};
	
	Mesh_ptr mesh(new Mesh());
	MeshData_ptr mdata(new MeshData());
	mdata->addVertices(posArray, 12);
	mdata->addNormal(normalArray, 12);
	mdata->addTextureCoords(texcoordArray, 8);
	mdata->addVertexIndexes(elementArray, 6);
	mesh->build(mdata);
	return mesh;
}


// ボックスのMeshクラスを作成します.
Mesh_ptr PrimitiveObject::createBox(Point3f size) {
	float x = size.x / 2.0;
	float y = size.y / 2.0;
	float z = size.z / 2.0;
	const GLfloat posArray[] = {
		 x, y, z,  -x, y, z,  -x,-y, z,   x,-y, z,  // v0-v1-v2-v3 front
		 x, y, z,   x,-y, z,   x,-y,-z,   x, y,-z,  // v0-v3-v4-v5 right
		 x, y, z,   x, y,-z,  -x, y,-z,  -x, y, z,  // v0-v5-v6-v1 top
		-x, y, z,  -x, y,-z,  -x,-y,-z,  -x,-y, z,  // v1-v6-v7-v2 left
		-x,-y,-z,   x,-y,-z,   x,-y, z,  -x,-y, z,  // v7-v4-v3-v2 bottom
		 x,-y,-z,  -x,-y,-z,  -x, y,-z,   x, y,-z   // v4-v7-v6-v5 back
	};
	
	const GLfloat texcoordArray[] = {
		1.0, 1.0,   0.0, 1.0,   0.0, 0.0,   1.0, 0.0,  // v0-v1-v2-v3 front
		0.0, 1.0,   0.0, 0.0,   1.0, 0.0,   1.0, 1.0,  // v0-v3-v4-v5 right
		1.0, 0.0,   1.0, 1.0,   0.0, 1.0,   0.0, 0.0,  // v0-v5-v6-v1 top
		1.0, 1.0,   0.0, 1.0,   0.0, 0.0,   1.0, 0.0,  // v1-v6-v7-v2 left
		0.0, 0.0,   1.0, 0.0,   1.0, 1.0,   0.0, 1.0,  // v7-v4-v3-v2 bottom
		0.0, 0.0,   1.0, 0.0,   1.0, 1.0,   0.0, 1.0   // v4-v7-v6-v5 back
	};
	
	const GLfloat normalArray[] = {
		 0, 0, 1,   0, 0, 1,   0, 0, 1,   0, 0, 1,   // v0-v1-v2-v3 front
		 1, 0, 0,   1, 0, 0,   1, 0, 0,   1, 0, 0,   // v0-v3-v4-v5 right
		 0, 1, 0,   0, 1, 0,   0, 1, 0,   0, 1, 0,   // v0-v5-v6-v1 top
		-1, 0, 0,  -1, 0, 0,  -1, 0, 0,  -1, 0, 0,   // v1-v6-v7-v2 left
		 0,-1, 0,   0,-1, 0,   0,-1, 0,   0,-1, 0,   // v7-v4-v3-v2 bottom
		 0, 0,-1,   0, 0,-1,   0, 0,-1,   0, 0,-1    // v4-v7-v6-v5 back
	};
	
	const GLushort elementArray[] = {
		 0, 1, 2,   0, 2, 3,   // front
		 4, 5, 6,   4, 6, 7,   // right
		 8, 9,10,   8,10,11,   // top
		12,13,14,  12,14,15,   // left
		16,17,18,  16,18,19,   // bottom
		20,21,22,  20,22,23    // back
	};
	
	Mesh_ptr mesh(new Mesh());
	MeshData_ptr mdata(new MeshData());
	mdata->addVertices(posArray, 72);
	mdata->addNormal(normalArray, 72);
	mdata->addTextureCoords(texcoordArray, 48);
	mdata->addVertexIndexes(elementArray, 36);
	mesh->build(mdata);
	return mesh;
}

