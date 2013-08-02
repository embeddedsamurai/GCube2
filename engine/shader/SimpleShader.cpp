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

#include "SimpleShader.h"
#include "../scene/Mesh.h"
#include "../scene/Camera.h"
#include "../scene/Figure.h"

using namespace GCube;

#define CONST_STR(name, str) static const char name[]=#str

CONST_STR(gVertexShader,

uniform mat4 u_mvpMatrix;
attribute vec3 a_position;
attribute vec2 a_texcoord;
varying vec2 v_texcoord;

void main()
{
	v_texcoord = a_texcoord;
	gl_Position = u_mvpMatrix * vec4(a_position, 1.0);
}

);

CONST_STR(gFragmentShader,

precision mediump float;

uniform sampler2D u_diffuseTexture;
uniform float u_bright;
uniform float u_alpha;
varying vec2 v_texcoord;

void main()
{
	gl_FragColor = vec4(texture2D(u_diffuseTexture, v_texcoord.st)) * vec4(vec3(u_bright), u_alpha);
}

);


// uniform index
enum {
	UNIFORM_MVP_MATRIX,   //!< MVPマトリクス変数へのユニフォーム
	UNIFORM_TEXTURE_BASE, //!< テクスチャへのユニフォーム
	UNIFORM_ALPHA,        //!< アルファ値へのユニフォーム
	UNIFORM_BRIGHT,       //!< ブライトネス値へのユニフォーム
	NUM_UNIFORMS          //!< ユニフォーム数
};
static GLint uniforms[NUM_UNIFORMS];

SimpleShader::SimpleShader() {
//	gProgram = loadShader("shader/simpleShader", 0);
	gProgram = loadShader(gVertexShader, gFragmentShader, 0);
	baseAlpha = 1.0;
	texname = -1;
}

SimpleShader::~SimpleShader() {
	if (gProgram) {
		if (glIsProgram(gProgram)==GL_TRUE) {
			glDeleteProgram(gProgram);
		}
	}
}

void SimpleShader::useProgram() {
	glUseProgram(gProgram);
	this->setAlpha(1.0);
	this->texname = -1;
}

void SimpleShader::setInfo(Figure *figure, Camera *camera, Scene *scene) {
	// projectionMatrix
	Matrix3D mtx;
	mtx.multiply(&camera->projectionMatrix);
	
	// viewMatrix
	camera->updateViewMatrix();
	mtx.multiply(&camera->viewMatrix);
	
	// modelMatrix
	mtx.multiply(&figure->transform);
	
	//
	glUniformMatrix4fv(uniforms[UNIFORM_MVP_MATRIX], 1, GL_FALSE, mtx.matrix);
}

void SimpleShader::bindTexture(int texname) {
	if (this->texname == texname) {
		// 前回bindしたテクスチャと同じ場合には何も処理を行わない
		return;
	}
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texname);
	glUniform1i(uniforms[UNIFORM_TEXTURE_BASE], 0);
	
	this->texname = texname;
}

void SimpleShader::setAlpha(float a) {
	glUniform1f(uniforms[UNIFORM_ALPHA], a * baseAlpha);
	glUniform1f(uniforms[UNIFORM_BRIGHT], a);
}

void SimpleShader::setBright(float b) {
	glUniform1f(uniforms[UNIFORM_BRIGHT], b);
}

void SimpleShader::bindAttribute(GLuint program, const char *name, int user) {
	glBindAttribLocation(program, ATTRIB_VERTEX, "a_position");
	glBindAttribLocation(program, ATTRIB_TEXCOORD, "a_texcoord");
}

void SimpleShader::getUniform(GLuint program, const char *name, int user) {
	uniforms[UNIFORM_MVP_MATRIX] = glGetUniformLocation(program, "u_mvpMatrix");
	uniforms[UNIFORM_TEXTURE_BASE] = glGetUniformLocation(program, "u_diffuseTexture");
	uniforms[UNIFORM_ALPHA] = glGetUniformLocation(program, "u_alpha");
	uniforms[UNIFORM_BRIGHT] = glGetUniformLocation(program, "u_bright");
}

void SimpleShader::setBaseAlpha(float baseAlpha) {
	this->baseAlpha = baseAlpha;
}

