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

#include "ColorShader.h"
#include "../scene/Mesh.h"
#include "../scene/Camera.h"
#include "../scene/Figure.h"
#include "../util/Log.h"

using namespace GCube;

//////////////////////////////////////////////////////////
// vertex shader
CONST_STR(gVertexShader,
		  
uniform mat4 u_mvpMatrix;
uniform vec4 u_color;
attribute vec3 a_position;
varying vec4 v_color;

void main()
{
	v_color = u_color;
	gl_Position = u_mvpMatrix * vec4(a_position, 1.0);
}

);

// fragment shader
CONST_STR(gFragmentShader,
		  
precision mediump float;
varying vec4 v_color;

void main()
{
	gl_FragColor = v_color;
}

);

//////////////////////////////////////////////////////////

ColorShader::ColorShader() {
	this->reload();
}

void ColorShader::reload() {
	gProgram = loadShader(gVertexShader, gFragmentShader);
}

void ColorShader::setInfo(Figure *figure, Camera *camera) {
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
	if (figure->material) {
		Colorf color = figure->material->ambientColor;
		glUniform4f(uniforms[UNIFORM_COLOR], color.r, color.g, color.b, color.a);
	}
}

void ColorShader::prepareShader(GLuint program) {
	attribs[AttribTypeVertex] = glGetAttribLocation(program, "a_position");
	
	uniforms[UNIFORM_MVP_MATRIX] = glGetUniformLocation(program, "u_mvpMatrix");
	uniforms[UNIFORM_COLOR] = glGetUniformLocation(program, "u_color");
}

