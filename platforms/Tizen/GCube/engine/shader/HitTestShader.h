//
//  HitTestShader.h
//  GCube
//
//  Created by Takashi Tsuchiya on 2013/08/29.
//  Copyright (c) 2013年 GClue, Inc. All rights reserved.
//

#ifndef __GCube__HitTestShader__
#define __GCube__HitTestShader__


#include "GCDefines.h"
#include "ColorShader.h"

namespace GCube {

/**
 * HitTest用塗りつぶしシェーダークラス.
 */
class HitTestShader : public ColorShader {
private:
	int count;
	
public:
	/**
	 * コンストラクタ.
	 */
	HitTestShader();
	
	/**
	 * デストラクタ.
	 */
	virtual ~HitTestShader() {};
	
	/**
	 * 各種情報を設定します.
	 */
	virtual void setInfo(Figure *figure, Camera *camera);
	
	/**
	 * リセットします.
	 */
	virtual void reset();
};

DEF_SHARED_PTR(HitTestShader);
}

#endif /* defined(__GCube__HitTestShader__) */
