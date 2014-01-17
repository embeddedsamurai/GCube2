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

#include "HitTestShader.h"
#include "../scene/Mesh.h"
#include "../scene/Camera.h"
#include "../scene/Figure.h"
#include "../util/Log.h"

using namespace GCube;

HitTestShader::HitTestShader() {
	this->reload();
	this->reset();
}

void HitTestShader::setExtraInfo(DrawContext &context, Figure &figure) {
	if (figure.isTouchable) {
		// 色作成
		count++;
		int r = count >> 16 & 0xff;
		int g = count >>  8 & 0xff;
		int b = count       & 0xff;
		Colorf color(r/255.0, g/255.0, b/255.0, 1);
		//LOGD("%d, %d, %d", r, g, b);
		glUniform4f(colorUniform, color.r, color.g, color.b, color.a);
		figure.testColor = color;
		context.color = color;
	} else {
//		Colorf color(0, 0, 0, 1);
		Colorf color = context.color;
		glUniform4f(colorUniform, color.r, color.g, color.b, color.a);
		figure.testColor = color;
	}
}

void HitTestShader::reset() {
	count = 0;
}
