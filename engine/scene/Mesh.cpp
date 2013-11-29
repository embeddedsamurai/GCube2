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

//////////////// MeshData

// 頂点追加
void MeshData::addVertices(const float *v, int len) {
	std::vector<float> tmp(v, v+len);
	vertices.insert(vertices.end(), tmp.begin(), tmp.end());
}

// 法線追加
void MeshData::addNormal(const float *v, int len) {
	std::vector<float> tmp(v, v+len);
	normals.insert(normals.end(), tmp.begin(), tmp.end());
}

// uv追加
void MeshData::addTextureCoords(const float *v, int len, bool mlt) {
	std::vector<float> tmp(v, v+len);
	if (mlt) {
		mltTextureCoords.insert(mltTextureCoords.end(), tmp.begin(), tmp.end());
	} else {
		textureCoords.insert(textureCoords.end(), tmp.begin(), tmp.end());
	}
}

// インデックス追加
void MeshData::addVertexIndexes(const unsigned short *v, int len) {
	std::vector<unsigned short> tmp(v, v+len);
	vertexIndexes.insert(vertexIndexes.end(), tmp.begin(), tmp.end());
}

// ジョイント追加
void MeshData::addJoints(const unsigned short *j1, const float *w1, const unsigned short *j2, const float *w2, int len) {
	for (int i = 0; i < len; i++) {
		float max = w1[i] + w2[i];
		jointData.push_back(j1[i]);
		jointData.push_back(w1[i]/max);
		jointData.push_back(j2[i]);
		jointData.push_back(w2[i]/max);
	}
}


//////////////// Mesh

// コンストラクタ
Mesh::Mesh() : indexCount(0) {
	vbo = Vbo_ptr(new Vbo());
}

// ビルド
void Mesh::build(const MeshData_ptr &data) {
	if (data->vertices.size()==0 || data->vertexIndexes.size()==0) {
		return;
	}
	vbo->addElement(data->vertices, AttribTypeVertex);
	if (data->normals.size()) vbo->addElement(data->normals, AttribTypeNormal);
	if (data->textureCoords.size()) vbo->addElement(data->textureCoords, AttribTypeUV0);
	if (data->mltTextureCoords.size()) vbo->addElement(data->mltTextureCoords, AttribTypeUV1);
	vbo->setIndex(data->vertexIndexes);
	indexCount = data->vertexIndexes.size();
	vbo->build();
}

// ビルド（インターリーブ）
void Mesh::build(const MeshInterleaveData_ptr &data) {
	vbo->setInterleaveData(data->data, data->attribs);
	vbo->setIndex(data->vertexIndexes);
	indexCount = data->vertexIndexes.size();
	vbo->build();
}

// 再構築
void Mesh::rebuild() {
	vbo->rebuild();
}

// バインド
void Mesh::bind(const Shader_ptr &shader) {
	vbo->bind(shader);
}

// サイズを取得
int Mesh::getIndexCount() {
	return indexCount;
}
