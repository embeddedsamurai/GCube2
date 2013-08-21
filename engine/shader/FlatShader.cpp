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

#include "FlatShader.h"
#include "../ApplicationController.h"
#include "../scene/Mesh.h"
#include "../scene/Camera.h"
#include "../scene/Figure.h"
#include "../scene/Scene.h"

using namespace GCube;

// vertex shader
CONST_STR(gVertexShader,

uniform mat4 u_mMatrix;
uniform mat4 u_mvpMatrix;
uniform mat4 u_texMatrix;
uniform mat3 u_nMatrix;
uniform int u_lightCount;
uniform vec3 u_lightPos[10];
uniform vec3 u_lightDiffuse[10];
attribute vec3 a_position;
attribute vec3 a_normal;
attribute vec2 a_texcoord;
varying vec2 v_texcoord;
varying vec3 v_color;

void main()
{
	// light
	vec4 pos = u_mMatrix * vec4(a_position, 1.0);
	vec3 normal = normalize(u_nMatrix * a_normal);
	v_color = vec3(0);
	for( int i = 0; i < u_lightCount; i++ ) {
		vec3 lightDir = normalize(u_lightPos[i] - vec3(pos));
		v_color += vec3(dot(lightDir, normal)) * u_lightDiffuse[i];
	}
	// tex
    v_texcoord = vec2(u_texMatrix * vec4(a_texcoord, 0, 1));
	// pos
	gl_Position = u_mvpMatrix * vec4(a_position, 1.0);
}

);

// fragment shader
CONST_STR(gFragmentShader,

precision mediump float;
uniform sampler2D u_texture;
varying vec2 v_texcoord;
varying vec3 v_color;

void main()
{
	gl_FragColor = vec4(texture2D(u_texture, vec2(v_texcoord.st))) * vec4(v_color, 1.0);
}

);


// uniform index
enum {
	UNIFORM_MVP_MATRIX,   //!< MVP変換行列
	UNIFORM_M_MATRIX,     //!< M変換行列
	UNIFORM_TEXTURE,      //!< テクスチャ
	UNIFORM_TEX_MATRIX,   //!< テクスチャ変換行列
	UNIFORM_NORMAL_MATRIX,//!< 法線マトリックス
	UNIFORM_LIGHT_POS,    //!< ライトの位置
	UNIFORM_LIGHT_AMBIENT,//!< ライトの環境光色
	UNIFORM_LIGHT_DIFFUSE,//!< ライトの拡散光色
	UNIFORM_LIGHT_COUNT,  //!< ライトの数
	NUM_UNIFORMS          //!< ユニフォーム数
};
static GLint uniforms[NUM_UNIFORMS];



FlatShader::FlatShader() {
	this->reload();
}

void FlatShader::reload() {
	gProgram = loadShader(gVertexShader, gFragmentShader, 0);
}

void FlatShader::setInfo(Figure *figure, Camera *camera) {
	// projectionMatrix
	if (!figure || !camera) return;
	Matrix3D mvp(camera->projectionMatrix);
	
	// viewMatrix
	camera->updateViewMatrix();
	mvp.multiply(&camera->viewMatrix);
	
	// modelMatrix
	mvp.multiply(&figure->globalMatrix);
	
	glUniformMatrix4fv(uniforms[UNIFORM_M_MATRIX], 1, GL_FALSE, figure->globalMatrix.matrix);
	glUniformMatrix4fv(uniforms[UNIFORM_MVP_MATRIX], 1, GL_FALSE, mvp.matrix);
	
	// テクスチャ
	if (figure->material && figure->material->texture) {
		figure->material->texture->bind();
		glActiveTexture(GL_TEXTURE0);
		glUniform1i(uniforms[UNIFORM_TEXTURE], 0);
		// 変換行列
		glUniformMatrix4fv(uniforms[UNIFORM_TEX_MATRIX], 1, GL_FALSE, figure->material->texture->matrix.matrix);
	}
	
	// 法線
	GLfloat normalMtx[9];
	figure->globalMatrix.normalMatrix(normalMtx);
	glUniformMatrix3fv(uniforms[UNIFORM_NORMAL_MATRIX], 1, GL_FALSE, normalMtx);

	// ライト
	ApplicationController *ctr = ApplicationController::SharedInstance();
	std::vector<Node*> lights = ctr->getActiveScene()->getLights();
	std::vector<Node*>::iterator it = lights.begin();
	GLfloat lightPos[lights.size()*3];
	GLfloat lightDif[lights.size()*3];
	int count = 0;
	while (it != lights.end()) {
		Light *l = (Light*) *it;
		int idx = count * 3;
		lightPos[idx  ] = l->globalMatrix.getX();
		lightPos[idx+1] = l->globalMatrix.getY();
		lightPos[idx+2] = l->globalMatrix.getZ();
		lightDif[idx  ] = l->diffuse.r;
		lightDif[idx+1] = l->diffuse.g;
		lightDif[idx+2] = l->diffuse.b;
		count++;
		it++;
	}
	glUniform1i(uniforms[UNIFORM_LIGHT_COUNT], count);
	glUniform3fv(uniforms[UNIFORM_LIGHT_POS], count, lightPos);
	glUniform3fv(uniforms[UNIFORM_LIGHT_DIFFUSE], count, lightDif);
}

void FlatShader::bindAttribute(GLuint program, const char *name, int user) {
	glBindAttribLocation(program, ATTRIB_VERTEX, "a_position");
	glBindAttribLocation(program, ATTRIB_TEXCOORD, "a_texcoord");
	glBindAttribLocation(program, ATTRIB_NORMAL, "a_normal");
}

void FlatShader::getUniform(GLuint program, const char *name, int user) {
	uniforms[UNIFORM_MVP_MATRIX] = glGetUniformLocation(program, "u_mvpMatrix");
	uniforms[UNIFORM_M_MATRIX] = glGetUniformLocation(program, "u_mMatrix");
	uniforms[UNIFORM_TEXTURE] = glGetUniformLocation(program, "u_texture");
	uniforms[UNIFORM_TEX_MATRIX] = glGetUniformLocation(program, "u_texMatrix");
	uniforms[UNIFORM_LIGHT_POS] = glGetUniformLocation(program, "u_lightPos");
	uniforms[UNIFORM_LIGHT_DIFFUSE] = glGetUniformLocation(program, "u_lightDiffuse");
	uniforms[UNIFORM_LIGHT_COUNT] = glGetUniformLocation(program, "u_lightCount");
	uniforms[UNIFORM_NORMAL_MATRIX] = glGetUniformLocation(program, "u_nMatrix");
}
