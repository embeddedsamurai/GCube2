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

#include "Figure.h"

using namespace GCube;

// コンストラクタ
Figure::Figure(const char *name) : TouchableNode(name) {
	type = FigureTypeID();
//	shader = NULL;
//	mesh = NULL;
}

// デストラクタ
Figure::~Figure() {
}

// 再構築
void Figure::rebuild() {
	mesh->rebuild();
}

// 描画
void Figure::draw(const Window &window, DrawType type) {
	if (isVisible && shader && mesh) {
		if (type==DrawTypeHitTest) {
			testShader->useProgram();
			mesh->bind();
			testShader->setInfo(this, window.camera.get());
		} else {
			shader->useProgram();
			mesh->bind();
			shader->setInfo(this, window.camera.get());
		}
		glDrawElements(GL_TRIANGLES, mesh->getIndexCount(), GL_UNSIGNED_SHORT, NULL);
	}
}
