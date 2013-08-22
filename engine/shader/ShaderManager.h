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

#ifndef __GCube__ShaderManager__
#define __GCube__ShaderManager__

#include "Shader.h"

namespace GCube {

typedef enum {
	ShaderTypeColor,
	ShaderTypeTex,
	ShaderTypeFlat,
	NUM_ShaderType
} ShaderType;

class ShaderManager {
public:
	ShaderManager() {};
	virtual ~ShaderManager() {};
	
	static Shader_ptr GetShader(ShaderType type);
	Shader_ptr getShader(ShaderType type);
//	static Shader_ptr GetCustomShader(int id);
//	static void AddCustomShader(Shader_ptr shader, int id);
	void reloadAllData();
	
public:
	std::map<int, Shader_ptr> cache; //!< キャッシュ.
};
}

#endif /* defined(__GCube__ShaderManager__) */
