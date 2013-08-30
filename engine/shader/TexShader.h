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

#ifndef __GCube__TexShader__
#define __GCube__TexShader__

#include "GCDefines.h"
#include "Shader.h"

namespace GCube {

/**
 * テクスチャ描画シェーダークラス.
 */
class TexShader : public Shader {
protected:
	// uniform index
	enum {
		UNIFORM_MVP_MATRIX, //!< MVP変換行列
		UNIFORM_TEXTURE,    //!< テクスチャ
		UNIFORM_TEX_MATRIX, //!< テクスチャ変換行列
		NUM_UNIFORMS        //!< ユニフォーム数
	};
	GLint uniforms[NUM_UNIFORMS];
	
public:
	/**
	 * コンストラクタ.
	 */
	TexShader();
	
	/**
	 * デストラクタ.
	 */
	virtual ~TexShader() {};
	
	/**
	 * 再読み込みします.
	 */
	virtual void reload();
	
	/**
	 * 各種情報を設定します.
	 */
	virtual void setInfo(Figure *figure, Camera *camera);
	
	/**
	 * シェーダのAttributeへのバインド処理を行います.
	 * <br><br>
	 * ES2Rendererから継承した関数.
	 * ES2Renderer内部から呼び出されるので、実装しないとエラーになります.
	 * <br>
	 * @param[in] program シェーダプログラム
	 * @param[in] name シェーダファイルへの名前
	 * @param[in] user ユーザ識別ID
	 */
	void bindAttribute(GLuint program, const char *name, int user);
	
	/**
	 * シェーダのUniformを取得します.
	 * <br><br>
	 * ES2Rendererから継承した関数.
	 * ES2Renderer内部から呼び出されるので、実装しないとエラーになります.
	 * <br>
	 * @param[in] program シェーダプログラム
	 * @param[in] name シェーダファイルへの名前
	 * @param[in] user ユーザ識別ID
	 */
	void getUniform(GLuint program, const char *name, int user);
	
};

DEF_SHARED_PTR(TexShader);
}

#endif /* defined(__GCube__TexShader__) */
