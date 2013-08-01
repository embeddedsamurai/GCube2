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

#ifndef __GCube__Matrix3D__
#define __GCube__Matrix3D__

#include <stddef.h>
#include "Vector3D.h"

namespace GCube {

/**
 * 回転軸を定義します.
 */
typedef enum {
	RotateDirX,		//!< x軸に回転.
	RotateDirY,		//!< y軸に回転.
	RotateDirZ,		//!< z軸に回転.
} RotateDir;

/**
 * 4x4マトリクスの計算を行うクラス.
 */
class Matrix3D {
public:
	float matrix[16];	//!< 4x4マトリクス.
    
	/**
	 * 単位行列で初期化.
	 */
	Matrix3D();
	
	/**
	 * 行列指定で初期化.
	 */
	Matrix3D(const float *inElements);
	
	/**
	 * デストラクタ.
	 */
	~Matrix3D();
	
	/**
	 * 行列を設定.
	 */
	void setElements(const float *inElements);
	
	/**
	 * マトリクスの初期化を行います.
	 */
	void loadIdentity();

	/**
	 * 指定されたmatrixと内容が等しいかを返します.
	 * <br><br>
	 * @param[in] mtx 対象のMatrix3D
	 * @return 等しい場合はtrue
	 */
	bool equals(Matrix3D *mtx);

	/**
	 * 指定されたmtxをかけ算します.
	 * <br><br>
	 * @param[in] mtx かけ算を行うMatrix3D
	 */
	void multiply(Matrix3D *mtx);

	/**
	 * 逆行列に変換します.
	 */
	void invert();
	
	/**
	 * 指定された座標に移動します.
	 * @param[in] x x座標
	 * @param[in] y y座標
	 * @param[in] z z座標
	 * @param[in] apply falseで右辺に乗算します.
	 */
	void translate(float x, float y, float z, bool apply=true);
	
	/**
	 * 指定された軸を向きに回転を行います.
	 * @param[in] deg 回転(0〜360)
	 * @param[in] dir 回転軸
	 * @param[in] apply falseで右辺に乗算します.
	 */
	void rotate(float deg, RotateDir dir, bool apply=true);
	
	/**
	 * 指定された軸を向きに回転を行います.
	 * @param[in] angle 回転(0〜360)
	 * @param[in] x 回転軸
	 * @param[in] y 回転軸
	 * @param[in] z 回転軸
	 * @param[in] apply falseで右辺に乗算します.
	 */
	void rotate(float angle, float x, float y, float z, bool apply=true);
	
	/**
	 * 拡大/縮小を行います.
	 * @param[in] x x軸への拡大
	 * @param[in] y y軸への拡大
	 * @param[in] z z軸への拡大
	 * @param[in] apply falseで右辺に乗算します.
	 */
	void scale(float x, float y, float z, bool apply=true);
	
	/**
	 * 法線ベクトルのマトリクスを取得します.
	 * @param[out] mtxout マトリクスを格納する3x3の配列
	 */
	void normalMatrix(float *mtxout);
	
	/**
	 * 変換行列を適用したVector3Dを返します.
	 * @param[out] outVec 座標を格納するVector3D
	 * @param[in] inVec 座標を格納するVector3D
	 */
	void transformVector3D(Vector3D *outVec, const Vector3D *inVec=NULL);
	
	/**
	 * 指定の場所(eye)から指定の位置(at)を向かせる変換行列を設定します.
	 * @param[in] eye 視点位置
	 * @param[in] at 注視点位置
	 * @param[in] up カメラの上方向（デフォルトはYUP）
	 */
	void lookAt(Vector3D* eye, Vector3D* at, Vector3D* up=NULL);
	
	/**
	 * Xを返します.
	 * <br><br>
	 * @return X
	 */
	float getX();
	
	/**
	 * Yを返します.
	 * <br><br>
	 * @return Y
	 */
	float getY();
	
	/**
	 * Zを返します.
	 * <br><br>
	 * @return Z
	 */
	float getZ();
	
	/**
	 * デバッグログに行列の内容を出力します.
	 */
	void debugPrint();
};

}

#endif /* defined(__GCube__Matrix3D__) */
