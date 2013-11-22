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

#include "Shader.h"
#include <stdio.h>
#include <stdlib.h>
#include "../util/Log.h"

using namespace GCube;

Shader::Shader() : gProgram(0) {
	for (int i=0; i<NUM_ATTRIB_TYPE; i++) {
		attribs[i] = -1;
	}
}

Shader::~Shader() {
	// TODO: 破棄用メソッド作成
//	if (gProgram) {
//		if (glIsProgram(gProgram)==GL_TRUE) {
//			glDeleteProgram(gProgram);
//		}
//	}
}

int Shader::getAttribLocation(AttribType type) {
	return attribs[type];
}

GLuint Shader::loadShader(const char* vertexShader, const char* fragmentShader) {
	GLuint vertShader = 0;
	GLuint fragShader = 0;
	
	// 頂点(Vertex)シェーダ
	vertShader = compileShader(GL_VERTEX_SHADER, vertexShader);
	if (!vertShader) {
		LOGE("Failed to compile vertex shader");
		goto ERROR;
	}
	
	// ピクセル(Fragment)シェーダ
	fragShader = compileShader(GL_FRAGMENT_SHADER, fragmentShader);
	if (!fragShader) {
		LOGE("Failed to compile fragment shader");
		goto ERROR;
	}
	
	// Create shader program
	return createProgram(vertShader, fragShader);
	
ERROR:	// エラー時の処理
	if (vertShader) {
		glDeleteShader(vertShader);
	}
	if (fragShader) {
		glDeleteShader(fragShader);
	}
	return 0;
}

GLuint Shader::loadShader(const char* name) {
	GLuint vertShader = 0;
	GLuint fragShader = 0;
	
	char fileName[512];
	
	// 頂点(Vertex)シェーダ
	sprintf(fileName, "%s.vsh", name);
	vertShader = compileShaderFromFile(GL_VERTEX_SHADER, fileName);
	if (!vertShader) {
		LOGE("Failed to compile vertex shader");
		goto ERROR;
	}
	
	// ピクセル(Fragment)シェーダ
	sprintf(fileName, "%s.fsh", name);
	fragShader = compileShaderFromFile(GL_FRAGMENT_SHADER, fileName);
	if (!fragShader) {
		LOGE("Failed to compile fragment shader");
		goto ERROR;
	}
	
	return createProgram(vertShader, fragShader);
	
ERROR:	// エラー時の処理
	if (vertShader) {
		glDeleteShader(vertShader);
	}
	if (fragShader) {
		glDeleteShader(fragShader);
	}
	return 0;
}

void Shader::useProgram() {
	glUseProgram(gProgram);
}

////////////////////////////////////////////////////////////////
///
/// ここからはprivate関数
///
////////////////////////////////////////////////////////////////

GLuint Shader::createProgram(GLuint vertShader, GLuint fragShader) {
	GLuint program = 0;
	
	// Create shader program
	program = glCreateProgram();
	// Attach vertex shader to program
	glAttachShader(program, vertShader);
	// Attach fragment shader to program
	glAttachShader(program, fragShader);
	
	// Link program
	if (!linkProgram(program)) {
		goto ERROR;
	}
	
	// この関数は、virtual関数なので、継承したクラスのメソッドを実行します。
	prepareShader(program);
	
	// 後始末
	if (vertShader) {
		glDeleteShader(vertShader);
	}
	if (fragShader) {
		glDeleteShader(fragShader);
	}
	return program;
	
ERROR:	// エラー時の処理
	if (vertShader) {
		glDeleteShader(vertShader);
	}
	if (fragShader) {
		glDeleteShader(fragShader);
	}
	if (program) {
		glDeleteProgram(program);
	}
	return 0;
}

GLuint Shader::compileShaderFromFile(GLenum shaderType, const char* fileName) {
	// assetからファイルの読み込み
	std::vector<char> fdata;
	GCGetResourceData(fileName, fdata);
	if (fdata.size()==0) {
		return 0;
	}
	fdata.push_back('\0');
	const char *data = (const char *) &(fdata)[0];
    
    LOGD("file:%s", fileName);
	return compileShader(shaderType, data);
}

GLuint Shader::compileShader(GLenum shaderType, const char* source) {
	// シェーダのコンパイル処理
	GLint status;
	GLuint shader = glCreateShader(shaderType);
	glShaderSource(shader, 1, &source, 0);
	glCompileShader(shader);
	
	// コンパイル結果を取得
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	if (status == 0) {
		char *log;
		GLint length;
		/* get the shader info log */
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
		log = (char*)malloc(length);
		glGetShaderInfoLog(shader, length, &status, log);
		glDeleteShader(shader);
		LOGE("compile error. [%s]", log);
		free(log);
		return 0;
	}
	return shader;
}

bool Shader::linkProgram(GLuint prog) {
	GLint status;
	// プログラムをリンク
	glLinkProgram(prog);
	// リンク結果を取得
	glGetProgramiv(prog, GL_LINK_STATUS, &status);
	if (status == 0) {
		GLint length;
		char *log;
		/* get the program info log */
		glGetProgramiv(prog, GL_INFO_LOG_LENGTH, &length);
		log = (char*)malloc(length);
		glGetProgramInfoLog(prog, length, &status, log);
		LOGE("link error. [%s]", log);
		free(log);
		return false;
	}
	return true;
}

