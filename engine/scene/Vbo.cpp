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

#include "Vbo.h"

using namespace GCube;

// コンストラクタ
Vbo::Vbo() {
	maxSize = 0;
	for (int i=0; i<NUM_VBO; i++) {
		vboNames[i] = 0;
	}
}

// デストラクタ
Vbo::~Vbo() {
	// VBO破棄
	// TODO: 破棄用メソッド作成
//	for (int i = 0; i < NUM_VBO; i++) {
//		if (vboNames[i]) {
//			glDeleteBuffers(1, &vboNames[i]);
//			vboNames[i] = 0;
//		}
//	}
}

// Attributeのサイズを取得
int Vbo::getAttribSize(AttribType type) {
	switch (type) {
		case AttribTypeUV:
		case AttribTypeUV2:
		case AttribTypeUV3:
		case AttribTypeUV4:
			return 2;
		default:
			return 3;
	}
}

// ビルド
void Vbo::build() {
	// ビルドは一回だけ
  	if (interleaveArray.size()!=0) {
		this->rebuild();
		return;
	}
	
	// interleaveにデータを並べ替え
	int readPos[dataArray.size()];
	maxSize = 0;
	for(int j = 0; j < dataArray.size(); j++) {
		readPos[j] = 0;
		maxSize += getAttribSize(attribArray[j]);
	}
//	for (int i=0; i<indexesArray.size(); i++) {
	for (int i=0; i<dataArray[0].size()/3; i++) { // TODO:頂点数/3
		for(int j = 0; j < dataArray.size(); j++) {
			int size = getAttribSize(attribArray[j]);
			for (int k=0; k<size; k++) {
				float data = dataArray[j][readPos[j]++];
				interleaveArray.insert(interleaveArray.end(), data);
			}
		}
	}
	// データ削除
	dataArray.clear();
	// VBO作成
	this->rebuild();
}

// 再構築
void Vbo::rebuild() {
	// VBO作成
	vboNames[VBO_INTERLEAVE] = buildVBO(&interleaveArray.front(),
										interleaveArray.size() * sizeof(float), GL_ARRAY_BUFFER);

	// インデックスのVBO作成
	vboNames[VBO_ELEMENT] = buildVBO(&indexesArray.front(),
									 indexesArray.size() * sizeof(unsigned short), GL_ELEMENT_ARRAY_BUFFER);
}

// バインド
void Vbo::bind(const Shader_ptr &shader) {
	// dataのVBOをバインド
	glBindBuffer(GL_ARRAY_BUFFER, vboNames[VBO_INTERLEAVE]);
	int pos = 0;
	for(int j = 0; j < attribArray.size(); j++) {
		AttribType type = attribArray[j];
		int size = getAttribSize(type);
		int loc = shader->getAttribLocation(type);
		if (loc>=0) {
			glEnableVertexAttribArray(loc);
			glVertexAttribPointer(loc, size, GL_FLOAT, GL_FALSE, sizeof(GLfloat)*maxSize, (GLfloat*)0+pos);
		}
		pos += size;
	}
	// indexのVBOをバインド
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboNames[VBO_ELEMENT]);
}

// 要素追加
void Vbo::addElement(const std::vector<float> &data, AttribType type) {
	dataArray.push_back(data);
	attribArray.push_back(type);
}

// 要素追加（インターリーブ）
void Vbo::setInterleaveData(const std::vector<float> &data, const std::vector<AttribType> &attribs) {
	interleaveArray = std::vector<float>(data);
	attribArray = std::vector<AttribType>(attribs);
	//
	maxSize = 0;
	for(int j = 0; j < attribArray.size(); j++) {
		maxSize += getAttribSize(attribArray[j]);
	}
}

// インデックス設定
void Vbo::setIndex(const std::vector<short> &index) {
	indexesArray = index;
}

// VBO作成
GLuint Vbo::buildVBO(void *data, int size, GLenum buffer) {
	GLuint buffName;
	
	// Create a vertex buffer object (VBO)
	glGenBuffers(1, &buffName);
	glBindBuffer(buffer, buffName);
	
	// Allocate and load data into the VBO
	glBufferData(buffer, size, data, GL_STATIC_DRAW);
	return buffName;
}
