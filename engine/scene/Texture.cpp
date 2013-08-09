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

#include "Texture.h"
#include "../ApplicationController.h"
#include "../external/stb/stb_image.h"
#include "../util/Log.h"

using namespace GCube;

Texture::Texture(const char *fname, bool useMipmap) : useMipmap(useMipmap) {
	filename.assign(fname);
	this->reload();
	// 基本読み込んだ画像は左上原点なので、反転しておく。
	this->matrix.scale(1, -1, 1);
}

void Texture::bind() {
	glBindTexture(GL_TEXTURE_2D, id);
}

void Texture::clamp() {
	glBindTexture(GL_TEXTURE_2D, id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}

void Texture::wrap() {
	glBindTexture(GL_TEXTURE_2D, id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
}

void Texture::reload() {
	id = 0;
	size = {0};
	
	// リソース読み込み
	ApplicationController *ctr = ApplicationController::SharedInstance();
	std::vector<char> buf;
	ctr->getResource(filename.c_str(), buf);
	if (buf.size()==0) return;
	
	// デコード
	unsigned char *pixels = NULL;
	int w, h, bpp;
	pixels = stbi_load_from_memory((const unsigned char *)&buf[0], buf.size(), &w, &h, &bpp, 4);
	size.width = w;
	size.height = h;
	//LOGD("s:%d-%d(%d)", w, h, bpp);
	
	// テクスチャ作成
	GLuint texName = 0;
	glGenTextures(1, &texName);
	if (!texName) {
		LOGE("** ERROR (create texture) ** %s", texName);
		if(pixels) free(pixels);
		exit(1);
	}
	id = texName;
	glBindTexture(GL_TEXTURE_2D, id);
	
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

	if (useMipmap) {
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glGenerateMipmap(GL_TEXTURE_2D);
	} else {
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	}
	
	if(pixels) free(pixels);
}
