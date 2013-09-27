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
