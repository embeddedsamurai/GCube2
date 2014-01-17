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

#ifndef __GCube__GCObjLoader__
#define __GCube__GCObjLoader__

#include "../../GCDefines.h"
#include "../../scene/Figure.h"

namespace GCube {

/**
 * GCubeオリジナル形式の3Dオブジェクト読み込みクラス.
 */
class GCObjLoader {
private:
	
public:
	/**
	 * ファイルから読み込みます.
	 * @param fileName ファイル名
	 */
	static Figure_ptr loadFile(const char *fileName, bool rightHanded=true);
	
	/**
	 * データから読み込みます.
	 * @param data データ
	 */
	static Figure_ptr loadData(std::vector<char>* data, bool rightHanded=true);
	
	
	static Mesh_ptr meshProcess(std::istringstream &stream);

};

}

#endif /* defined(__GCube__GCObjLoader__) */
