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

#ifndef __GCube__Vbo__
#define __GCube__Vbo__

#include "GCDefines.h"

namespace GCube {

// vbo index
enum {
	VBO_INTERLEAVE,
	VBO_ELEMENT,
	NUM_VBO
};


class Vbo {
public:
	Vbo();
	virtual ~Vbo();
	
	virtual void addElement(const std::vector<float> &data, int element);
	virtual void setIndex(const std::vector<short> &index);
	
	virtual void build();
	virtual void bind();

private:
	GLuint buildVBO(void *data, int size, GLenum buffer);
	
	int maxSize;
	GLuint vboNames[NUM_VBO];                   //!< VBOの名前リスト.
	std::vector<std::vector<float>> dataArray;  //!< データ.
	std::vector<short> indexesArray;            //!< インデックスデータ.
	std::vector<int> elementsArray;             //!< 要素数データ.
	
};
DEF_SHARED_PTR(Vbo);
}

#endif /* defined(__GCube__Vbo__) */
