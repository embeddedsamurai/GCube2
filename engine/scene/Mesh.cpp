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

#include "Mesh.h"

using namespace GCube;

// コンストラクタ
Mesh::Mesh() : indexCount(0) {
}

// ビルド
void Mesh::build(const MeshData_ptr &data) {
	if (data->vertices.size()==0 || data->vertexIndexes.size()==0) {
		return;
	}
	vbo->addElement(data->vertices, 3);
	if (data->normals.size()) vbo->addElement(data->normals, 3);
	if (data->textureCoords.size()) vbo->addElement(data->textureCoords, 2);
	if (data->mltTextureCoords.size()) vbo->addElement(data->mltTextureCoords, 2);
	if (data->jointData.size()) vbo->addElement(data->jointData, 4);
	vbo->setIndex(data->vertexIndexes);
	indexCount = data->vertexIndexes.size();
	vbo->build();
}

// バインド
void Mesh::bind() {
	vbo->bind();
}

// サイズを取得
int Mesh::getIndexCount() {
	return indexCount;
}
