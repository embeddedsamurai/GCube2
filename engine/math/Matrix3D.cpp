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

#include "Matrix3D.h"
#include <memory.h>
#include <string.h>
#include <math.h>
#include "../external/math/matrixUtil.h"
#include "../util/Log.h"

// matrixUtilの変換が間違っている気がするのでこちらで修正（１）
void mtxRotateYApplyFixed(float* mtx, float deg)
{
	// [ 0 4  8 12 ]   [ cos 0  sin  0 ]
	// [ 1 5  9 13 ] x [ 0   1  0    0 ]
	// [ 2 6 10 14 ]   [-sin 0  cos  0 ]
	// [ 3 7 11 15 ]   [ 0   0  0    1 ]
	
	float rad = deg * (M_PI/180.0f);
	
	float cosrad = cosf(rad);
	float sinrad = sinf(rad);
	
	float mtx00 = mtx[0];
	float mtx01 = mtx[1];
	float mtx02 = mtx[2];
	float mtx03 = mtx[3];
	
	mtx[ 0] = -mtx[ 8]*sinrad + mtx00*cosrad;
	mtx[ 8] =  mtx[ 8]*cosrad + mtx00*sinrad;
	
	mtx[ 1] = -mtx[ 9]*sinrad + mtx01*cosrad;
	mtx[ 9] =  mtx[ 9]*cosrad + mtx01*sinrad;
	
	mtx[ 2] = -mtx[10]*sinrad + mtx02*cosrad;
	mtx[10] =  mtx[10]*cosrad + mtx02*sinrad;
	
	mtx[ 3] = -mtx[11]*sinrad + mtx03*cosrad;
	mtx[11] =  mtx[11]*cosrad + mtx03*sinrad;
}

// matrixUtilの変換が間違っている気がするのでこちらで修正（２）
void mtxRotateYMatrixFixed(float* mtx, float rad)
{
	// [ cos 0  sin  0 ]   [ 0 4  8 12 ]
	// [ 0   1  0    0 ] x [ 1 5  9 13 ]
	// [-sin 0  cos  0 ]   [ 2 6 10 14 ]
	// [ 0   0  0    1 ]   [ 3 7 11 15 ]
	
	float cosrad = cosf(rad);
	float sinrad = sinf(rad);
	
	float mtx00 = mtx[ 0];
	float mtx04 = mtx[ 4];
	float mtx08 = mtx[ 8];
	float mtx12 = mtx[12];
	
	mtx[ 0] =  cosrad*mtx00 + sinrad*mtx[ 2];
	mtx[ 2] = -sinrad*mtx00 + cosrad*mtx[ 2];
	
	mtx[ 4] =  cosrad*mtx04 + sinrad*mtx[ 6];
	mtx[ 6] = -sinrad*mtx04 + cosrad*mtx[ 6];
	
	mtx[ 8] =  cosrad*mtx08 + sinrad*mtx[10];
	mtx[10] = -sinrad*mtx08 + cosrad*mtx[10];
	
	mtx[12] =  cosrad*mtx12 + sinrad*mtx[14];
	mtx[14] = -sinrad*mtx12 + cosrad*mtx[14];
}

// matrixUtilの変換が間違っている気がするのでこちらで修正（３）
void mtxLoadRotateFixed(float* mtx, float angle, float x, float y, float z)
{
	float rad = angle * M_PI/180.0f;

	float c = cosf(rad);
    float s = sinf(rad);
    float xx = x * x;
    float xy = x * y;
    float xz = x * z;
    float yy = y * y;
    float yz = y * z;
    float zz = z * z;
	
    mtx[0] = xx * (1 - c) + c;
    mtx[1] = xy * (1 - c) + z * s;
    mtx[2] = xz * (1 - c) - y * s;
    mtx[3] = 0;
    mtx[4] = xy * (1 - c) - z * s;
    mtx[5] = yy * (1 - c) + c;
    mtx[6] = yz * (1 - c) + x * s;
    mtx[7] = 0;
    mtx[8] = xz * (1 - c) + y * s;
    mtx[9] = yz * (1 - c) - x * s;
    mtx[10] = zz * (1 - c) + c;
    mtx[11] = 0;
    mtx[12] = 0;
    mtx[13] = 0;
    mtx[14] = 0;
    mtx[15] = 1;
}


using namespace GCube;

Matrix3D::Matrix3D() {
	loadIdentity();
}

Matrix3D::Matrix3D(const float *inElements) {
	setElements(inElements);
}

Matrix3D::~Matrix3D() {
}

// 配列を設定
void Matrix3D::setElements(const float *inElements) {
    memcpy(matrix, inElements, sizeof(float) * 16);
}

// 単位行列を設定
void Matrix3D::loadIdentity() {
	mtxLoadIdentity(matrix);
}

// 等しいか
bool Matrix3D::equals(Matrix3D *mtx) {
	for (int i=0; i<16; i++) {
		if (matrix[i]!=mtx->matrix[i]) return false;
	}
	return true;
}

// 乗算
void Matrix3D::multiply(Matrix3D *mtx) {
	float ret[16];
	mtxMultiply(ret, matrix, mtx->matrix);
	memcpy(matrix, ret, sizeof(float)*16);
}

// 逆行列
void Matrix3D::invert() {
	float ret[16];
	mtxInvert(ret, matrix);
	mtxTranspose(ret, ret);
	memcpy(matrix, ret, sizeof(float)*16);
}

// 平行移動
void Matrix3D::translate(float x, float y, float z, bool apply) {
	if (apply) {
		mtxTranslateApply(matrix, x, y, z);
	} else {
		mtxTranslateMatrix(matrix, x, y, z);
	}
}

// 回転
void Matrix3D::rotate(float deg, RotateDir dir, bool apply) {
	if (apply) {
		switch (dir) {
			case RotateDirX:
				mtxRotateXApply(matrix, deg);
				break;
			case RotateDirY:
				mtxRotateYApplyFixed(matrix, deg);
				break;
			case RotateDirZ:
				mtxRotateZApply(matrix, deg);
				break;
		}
	} else {
		float rad = deg * (M_PI / 180.0f);
		switch (dir) {
			case RotateDirX:
				mtxRotateXMatrix(matrix, rad);
				break;
			case RotateDirY:
				mtxRotateYMatrixFixed(matrix, rad);
				break;
			case RotateDirZ:
				mtxRotateZMatrix(matrix, rad);
				break;
		}
	}
}

// 回転
void Matrix3D::rotate(float deg, float x, float y, float z, bool apply) {
	float rotMtx[16];
	mtxLoadRotateFixed(rotMtx, deg, x, y, z);

	float ret[16];
	if (apply) {
		mtxMultiply(ret, matrix, rotMtx);
	} else {
		mtxMultiply(ret, rotMtx, matrix);
	}
	memcpy(matrix, ret, sizeof(float)*16);
}

// 拡大縮小
void Matrix3D::scale(float x, float y, float z, bool apply) {
	if (apply) {
		mtxScaleApply(matrix, x, y, z);
	} else {
		mtxScaleMatrix(matrix, x, y, z);
	}
}

// 法線ベクトル取得
void Matrix3D::normalMatrix(float *mtxout) {
	float tmp1[9];
	float tmp2[9];
	float normal[9];
	mtx3x3FromTopLeftOf4x4(tmp1, matrix);
	mtx3x3Invert(tmp2, tmp1);
	mtx3x3Transpose(normal, tmp2);
	memcpy(mtxout, normal, sizeof(float) * 9);
}

// 変換行列を適用したVector3Dを返します
void Matrix3D::transformVector3D(Vector3D *outVec, const Vector3D *inVec) {
	
	bool flg = false;
	if (!inVec) {
		inVec = new Vector3D();
		flg = true;
	}
	
	float tmp[4];
	for (int i=0; i<4; i++) {
		tmp[i] =
		(matrix[i   ] * inVec->x) +
		(matrix[i+ 4] * inVec->y) +
		(matrix[i+ 8] * inVec->z) +
		(matrix[i+12] * inVec->w);
	}
	outVec->x = tmp[0];
	outVec->y = tmp[1];
	outVec->z = tmp[2];
	outVec->w = tmp[3];
	
	if (flg) {
		delete inVec;
	}
}

// 指定の方向を向かせる
void Matrix3D::lookAt(Vector3D* eye, Vector3D* at, Vector3D* up) {
	
	Vector3D ra = Vector3D(at);
	ra.multiply(-1);
	
	Vector3D f = Vector3D(eye);
	f.multiply(-1.0);
	f.subtract(&ra);
	f.normalize();
	
	Vector3D upactual;
	if (up==NULL) {
		upactual = Vector3D(0, 1, 0);
	} else {
		upactual = Vector3D(up);
	}
	upactual.normalize();
	
	Vector3D s = Vector3D(f);
	s.crossProduct(&upactual);
	s.normalize();
	
	Vector3D u = Vector3D(s);
	u.crossProduct(&f);
	u.normalize();
	
	matrix[ 0] = s.x;
	matrix[ 1] = u.x;
	matrix[ 2] = -f.x;
	matrix[ 3] = 0;
	
	matrix[ 4] = s.y;
	matrix[ 5] = u.y;
	matrix[ 6] = -f.y;
	matrix[ 7] = 0;
	
	matrix[ 8] = s.z;
	matrix[ 9] = u.z;
	matrix[10] = -f.z;
	matrix[11] = 0;
	
	matrix[12] = 0;
	matrix[13] = 0;
	matrix[14] = 0;
	matrix[15] = 1;
	
	this->translate(eye->x, eye->y, eye->z, true);
}

// Xを返します.
float Matrix3D::getX() {
	return matrix[12];
}

// Yを返します.
float Matrix3D::getY() {
	return matrix[13];
}

// Zを返します.
float Matrix3D::getZ() {
	return matrix[14];
}

// デバッグログ
void Matrix3D::debugPrint() {
	LOGD("matrix:[%f,%f,%f,%f]", matrix[0], matrix[4], matrix[ 8], matrix[12]);
	LOGD("      :[%f,%f,%f,%f]", matrix[1], matrix[5], matrix[ 9], matrix[13]);
	LOGD("      :[%f,%f,%f,%f]", matrix[2], matrix[6], matrix[10], matrix[14]);
	LOGD("      :[%f,%f,%f,%f]", matrix[3], matrix[7], matrix[11], matrix[15]);
//	LOGD("matrix:[%f,%f,%f,%f][%f,%f,%f,%f][%f,%f,%f,%f][%f,%f,%f,%f]]", matrix[0], matrix[1], matrix[2], matrix[3], matrix[4], matrix[5], matrix[6], matrix[7], matrix[8], matrix[9], matrix[10], matrix[11], matrix[12], matrix[13], matrix[14], matrix[15]);
}
