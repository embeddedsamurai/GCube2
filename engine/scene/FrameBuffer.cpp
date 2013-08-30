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

#include "FrameBuffer.h"
#include "../util/Log.h"

using namespace GCube;

FrameBuffer::FrameBuffer() : framebufferID(0), colorRenderbufferID(0), depthRenderbufferID(0) {
	
}

void FrameBuffer::createBuffer(const Sizef &size, bool useDepthBuffer) {
	// HitTest用のフレームバッファ作成
	if (framebufferID) return;
	
	glGenFramebuffers(1, &framebufferID);
	glBindFramebuffer(GL_FRAMEBUFFER, framebufferID);
	
	//Create the RenderBuffer for offscreen rendering // Color
	glGenRenderbuffers(1, &colorRenderbufferID);
	glBindRenderbuffer(GL_RENDERBUFFER, colorRenderbufferID);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_RGBA8_OES, size.width, size.height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, colorRenderbufferID);
	
	if (useDepthBuffer) {
		glGenRenderbuffers(1, &depthRenderbufferID);
		glBindRenderbuffer(GL_RENDERBUFFER, depthRenderbufferID);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, size.width, size.height);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthRenderbufferID);
	}
	
	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status!=GL_FRAMEBUFFER_COMPLETE) {
		LOGD("*ERROR* FrameBuffer::createBuffer [%d]", status);
	}
	
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

void FrameBuffer::deleteBuffer() {
	if (framebufferID) return;

	glDeleteRenderbuffers(1, &depthRenderbufferID);
	glDeleteRenderbuffers(1, &colorRenderbufferID);
	glDeleteFramebuffers(1, &framebufferID);
	depthRenderbufferID = 0;
	colorRenderbufferID = 0;
	framebufferID = 0;
}

void FrameBuffer::bind() {
	glBindFramebuffer(GL_FRAMEBUFFER, framebufferID);
}
