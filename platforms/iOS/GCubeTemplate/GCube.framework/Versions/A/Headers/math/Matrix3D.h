//
//  Matrix3D.h
//  GCube
//
//  Created by Takashi Tsuchiya on 2013/07/19.
//  Copyright (c) 2013年 GClue, Inc. All rights reserved.
//

#ifndef __GCube__Matrix3D__
#define __GCube__Matrix3D__

namespace GCube {

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
	 * 指定行列で初期化.
	 */
	Matrix3D(const float *inElements);
	
	/**
	 * デストラクタ.
	 */
	~Matrix3D();
	
	/**
	 * マトリクスの初期化を行います.
	 */
	void loadIdentity();

	/**
	 * デバッグログに行列の内容を出力します.
	 */
	void debugPrint();
};

}

#endif /* defined(__GCube__Matrix3D__) */
