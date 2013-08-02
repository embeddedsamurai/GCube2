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
	for (int i = 0; i < NUM_VBO; i++) {
		if (vboNames[i]) {
			glDeleteBuffers(1, &vboNames[i]);
			vboNames[i] = 0;
		}
	}
}

// ビルド
void Vbo::build() {
	// ビルドは一回だけ
  	if (vboNames[0]!=0 || vboNames[1]!=0) {
		return;
	}
	
	// interleaveにデータを並べ替え
	std::vector<float> interleave;
	int readPos[dataArray.size()];
	for(int j = 0; j < dataArray.size(); j++) {
		readPos[j] = 0;
		maxSize += elementsArray[j];
	}
	for (int i=0; i<indexesArray.size(); i++) {
		for(int j = 0; j < dataArray.size(); j++) {
			int size = elementsArray[j];
			for (int k=0; k<size; k++) {
				float data = dataArray[j][readPos[j]++];
				interleave.insert(interleave.end(), data);
			}
		}
	}
	// TODO:データ削除
	
	// VBO作成
	vboNames[VBO_INTERLEAVE] = buildVBO(&interleave.front(),
										interleave.size() * sizeof(float), GL_ARRAY_BUFFER);

	// インデックスのVBO作成
	vboNames[VBO_ELEMENT] = buildVBO(&indexesArray.front(),
									 indexesArray.size() * sizeof(unsigned short), GL_ELEMENT_ARRAY_BUFFER);

	// Attribute設定
	int pos = 0;
	for(int j = 0; j < dataArray.size(); j++) {
		int size = elementsArray[j];
		glEnableVertexAttribArray(j);
		glVertexAttribPointer(j, size, GL_FLOAT, GL_FALSE, sizeof(GLfloat)*maxSize, (GLfloat*)0+pos);
		pos += size;
	}
	
}

// バインド
void Vbo::bind() {
	// dataのVBOをバインド
	glBindBuffer(GL_ARRAY_BUFFER, vboNames[VBO_INTERLEAVE]);
	int pos = 0;
	for(int j = 0; j < dataArray.size(); j++) {
		int size = elementsArray[j];
		glEnableVertexAttribArray(j);
		glVertexAttribPointer(j, size, GL_FLOAT, GL_FALSE, sizeof(GLfloat)*maxSize, (GLfloat*)0+pos);
		pos += size;
	}
	// indexのVBOをバインド
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboNames[VBO_ELEMENT]);
}

// 要素追加
void Vbo::addElement(const std::vector<float> &data, int element) {
	dataArray.push_back(data);
	elementsArray.push_back(element);
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
