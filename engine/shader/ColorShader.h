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

#ifndef __GCube__ColorShader__
#define __GCube__ColorShader__

#include "GCDefines.h"
#include "BaseShader.h"

namespace GCube {

/**
 * 単色塗りつぶしシェーダークラス.
 */
class ColorShader : public BaseShader {
protected:
	GLint colorUniform;
	
	/**
	 * シェーダーの準備を行います.
	 * <br><br>
	 * @param[in] program シェーダプログラムのID
	 */
	virtual void prepareExtraShader(GLuint program);

	/**
	 * 各種追加情報を設定します.
	 */
	virtual void setExtraInfo(Figure *figure, Camera *camera);
	
public:
	/**
	 * コンストラクタ.
	 */
	ColorShader();
	
	/**
	 * デストラクタ.
	 */
	virtual ~ColorShader() {};
	
	/**
	 * 再読み込みします.
	 */
	virtual void reload();

};

DEF_SHARED_PTR(ColorShader);
}

#endif /* defined(__GCube__ColorShader__) */
