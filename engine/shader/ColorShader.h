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
#include "Shader.h"

namespace GCube {

/**
 * 単色塗りつぶしシェーダークラス.
 */
class ColorShader : public Shader {
protected:
	// uniform index
	enum {
		UNIFORM_MVP_MATRIX,   //!< MVPマトリクス変数へのユニフォーム
		UNIFORM_COLOR,        //!< 描画色へのユニフォーム
		NUM_UNIFORMS          //!< ユニフォーム数
	};
	GLint uniforms[NUM_UNIFORMS];
	
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

	/**
	 * 各種情報を設定します.
	 */
	virtual void setInfo(Figure *figure, Camera *camera);
	
	/**
	 * シェーダーの準備を行います.
	 * UniformやAttributeのLocationを取得してください.
	 * <br><br>
	 *
	 * @param[in] program シェーダプログラムのID
	 */
	virtual void prepareShader(GLuint program);
};

DEF_SHARED_PTR(ColorShader);
}

#endif /* defined(__GCube__ColorShader__) */
