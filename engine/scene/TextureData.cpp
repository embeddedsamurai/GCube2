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

#include "TextureData.h"
#include "../ApplicationController.h"
#include "../external/stb/stb_image.h"
#include "../util/Log.h"

using namespace GCube;

static std::map<std::string, TextureData_wkptr> cacheMap;

TextureData_ptr TextureData::GetTextureData(const char *fname, bool useMipmap) {
	// キャッシュ存在チェック
	std::map<std::string, TextureData_wkptr>::iterator it = cacheMap.find(fname);
	if (it!=cacheMap.end()) {
		if ((*it).second.lock()) {
			return (*it).second.lock();
		}
	}
	
	// 新規作成
	TextureData_ptr tex = TextureData_ptr(new TextureData(fname, useMipmap));
	cacheMap[fname] = tex;
	return tex;
}

void TextureData::ReloadAllData() {
	std::map<std::string, TextureData_wkptr>::iterator it = cacheMap.begin();
	while (it != cacheMap.end()) {
		TextureData_wkptr tex = (*it).second;
		if (tex.lock()) {
			tex.lock()->reload();
		}
		it++;
	}
}

TextureData::TextureData(const char *fname, bool useMipmap) : useMipmap(useMipmap) {
	filename.assign(fname);
	this->reload();
}

void TextureData::reload() {
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
