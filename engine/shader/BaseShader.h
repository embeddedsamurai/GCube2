/*
 * The MIT License (MIT)
 * Copyright (c) 2011 GClue, inc.
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

#ifndef __GCube__BaseShader__
#define __GCube__BaseShader__


#include "GCDefines.h"
#include "Shader.h"

namespace GCube {

/**
 * 基底シェーダークラス.
 * 一般的なAttributeやUniformを設定します.
 */
class BaseShader : public Shader {
protected:
	GLint attribs[NUM_ATTRIB_TYPE];
	enum {
		UNIFORM_MV_MATRIX,    //!< ModelView変換行列
		UNIFORM_P_MATRIX,     //!< Projection変換行列
		UNIFORM_MVP_MATRIX,   //!< ModelViewProjection変換行列
		UNIFORM_NORMAL_MATRIX,//!< 法線マトリックス
		UNIFORM_TEX_MATRIX,   //!< テクスチャ変換行列
		UNIFORM_TEXTURE,      //!< テクスチャ
		UNIFORM_USE_TEXTURE,  //!< テクスチャ使用フラグ
		UNIFORM_MAX_LIGHT,    //!< ライトの数
		NUM_UNIFORMS          //!< ユニフォーム数
	};
	GLint uniforms[NUM_UNIFORMS];
	// テクスチャ（マルチテクスチャ用）
	GLint uniformTextures[kMaxTextureUnit];
	// テクスチャ変換行列（マルチテクスチャ用）
	GLint uniformTextureMatrix[kMaxTextureUnit];
	// ライト
	enum {
		UNIFORM_LIGHT_AMBIENT,  //!< ライトの環境光色
		UNIFORM_LIGHT_DIFFUSE,  //!< ライトの拡散光色
		UNIFORM_LIGHT_SPECULAR, //!< ライトの拡散光色
		UNIFORM_LIGHT_POS,      //!< ライトの位置
		NUM_LIGHT_UNIFORMS      //!< ユニフォーム数
	};
	GLint uniformLights[kMaxLightUnit][NUM_LIGHT_UNIFORMS];
	
	/**
	 * 各種情報を設定します.
	 * 標準のUniformやAttributeのLocationの設定をします.
	 */
	virtual void setInfo(Scene *scene, Figure *figure, Camera *camera);

	/**
	 * 各種追加情報を設定します.
	 * 標準では準備されていないUniformやAttributeの設定をしてください.
	 */
	virtual void setExtraInfo(Figure *figure, Camera *camera) {};

	/**
	 * シェーダーの準備を行います.
	 * 標準のUniformやAttributeのLocationを取得します.
	 * <br><br>
	 *
	 * @param[in] program シェーダプログラムのID
	 */
	virtual void prepareShader(GLuint program);
	
	/**
	 * シェーダーの準備を行います.
	 * 標準では準備されていないUniformやAttributeのLocationを取得してください.
	 * <br><br>
	 *
	 * このクラスを継承したクラスで、このメソッドを実装します。
	 * このメソッドは、loadShaderが実行されたときに呼び出されます。
	 *
	 * @param[in] program シェーダプログラムのID
	 */
	virtual void prepareExtraShader(GLuint program) {};
	
public:
	/**
	 * コンストラクタ.
	 */
	BaseShader();
	
	/**
	 * デストラクタ.
	 */
	virtual ~BaseShader() {};
	
	/**
	 * Attributeの場所を取得します.
	 * <br><br>
	 *
	 * @param[in] type Attributeのタイプ
	 */
	virtual int getAttribLocation(AttribType type);
	
};

DEF_SHARED_PTR(BaseShader);
}

#endif /* defined(__GCube__BaseShader__) */
