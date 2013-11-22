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

#include "TexShader.h"
#include "../scene/Mesh.h"
#include "../scene/Camera.h"
#include "../scene/Figure.h"

using namespace GCube;

//////////////////////////////////////////////////////////
// vertex shader
CONST_STR(gVertexShader,

uniform mat4 u_mvpMatrix;
uniform mat4 u_texMatrix;
attribute vec3 a_position;
attribute vec2 a_texcoord;
varying vec2 v_texcoord;

void main()
{
    v_texcoord = vec2(u_texMatrix * vec4(a_texcoord, 0, 1));
	gl_Position = u_mvpMatrix * vec4(a_position, 1.0);
}

);

// fragment shader
CONST_STR(gFragmentShader,

precision mediump float;
uniform sampler2D u_texture;
varying vec2 v_texcoord;

void main()
{
	gl_FragColor = vec4(texture2D(u_texture, vec2(v_texcoord.st)));
}

);

//////////////////////////////////////////////////////////


TexShader::TexShader() {
	this->reload();
}

void TexShader::reload() {
	gProgram = loadShader(gVertexShader, gFragmentShader);
}

void TexShader::setInfo(Figure *figure, Camera *camera) {
	// projectionMatrix
	if (!figure || !camera) return;
	Matrix3D mtx(camera->projectionMatrix);
	
	// viewMatrix
	camera->updateViewMatrix();
	mtx.multiply(&camera->viewMatrix);
	
	// modelMatrix
	mtx.multiply(&figure->globalMatrix);
	
	// ユニフォーム変数へ渡す
	glUniformMatrix4fv(uniforms[UNIFORM_MVP_MATRIX], 1, GL_FALSE, mtx.matrix);
	if (figure->material && figure->material->texture) {
		// テクスチャ
		figure->material->texture->bind();
		glActiveTexture(GL_TEXTURE0);
		glUniform1i(uniforms[UNIFORM_TEXTURE], 0);
		// 変換行列
		glUniformMatrix4fv(uniforms[UNIFORM_TEX_MATRIX], 1, GL_FALSE, figure->material->texture->matrix.matrix);
	}
}

void TexShader::prepareShader(GLuint program) {
	attribs[AttribTypeVertex] = glGetAttribLocation(program, "a_position");
	attribs[AttribTypeUV] = glGetAttribLocation(program, "a_texcoord");
	
	uniforms[UNIFORM_MVP_MATRIX] = glGetUniformLocation(program, "u_mvpMatrix");
	uniforms[UNIFORM_TEXTURE] = glGetUniformLocation(program, "u_texture");
	uniforms[UNIFORM_TEX_MATRIX] = glGetUniformLocation(program, "u_texMatrix");
}
