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

#include "BaseShader.h"
#include "../scene/Scene.h"
#include "../scene/Mesh.h"
#include "../scene/Camera.h"
#include "../scene/Figure.h"
#include "../util/Log.h"

using namespace GCube;

BaseShader::BaseShader() {
	for (int i=0; i<NUM_ATTRIB_TYPE; i++) {
		attribs[i] = -1;
	}
}

int BaseShader::getAttribLocation(AttribType type) {
	return attribs[type];
}

void BaseShader::prepareShader(GLuint program) {
	// attributes
	attribs[AttribTypeVertex] = glGetAttribLocation(program, "a_Vertex");
	attribs[AttribTypeNormal] = glGetAttribLocation(program, "a_Normal");
	attribs[AttribTypeColor] = glGetAttribLocation(program, "a_Color");
	attribs[AttribTypeUV0] = glGetAttribLocation(program, "a_MultiTexCoord0");
	attribs[AttribTypeUV1] = glGetAttribLocation(program, "a_MultiTexCoord1");
	attribs[AttribTypeUV2] = glGetAttribLocation(program, "a_MultiTexCoord2");
//	attribs[AttribTypeUV3] = glGetAttribLocation(program, "a_MultiTexCoord3");
//	attribs[AttribTypeUV4] = glGetAttribLocation(program, "a_MultiTexCoord4");
//	attribs[AttribTypeUV5] = glGetAttribLocation(program, "a_MultiTexCoord5");
//	attribs[AttribTypeUV6] = glGetAttribLocation(program, "a_MultiTexCoord6");
//	attribs[AttribTypeUV7] = glGetAttribLocation(program, "a_MultiTexCoord7");
	attribs[AttribTypeFogCoord] = glGetAttribLocation(program, "a_FogCoord");
	// uniforms
	uniforms[UNIFORM_MV_MATRIX] = glGetUniformLocation(program, "u_ModelViewMatrix");
	uniforms[UNIFORM_P_MATRIX] = glGetUniformLocation(program, "u_ProjectionMatrix");
	uniforms[UNIFORM_MVP_MATRIX] = glGetUniformLocation(program, "u_ModelViewProjectionMatrix");
	uniforms[UNIFORM_NORMAL_MATRIX] = glGetUniformLocation(program, "u_NormalMatrix");
	uniforms[UNIFORM_TEX_MATRIX] = glGetUniformLocation(program, "u_TextureMatrix");
	uniforms[UNIFORM_TEXTURE] = glGetUniformLocation(program, "u_Texture");
	uniforms[UNIFORM_USE_TEXTURE] = glGetUniformLocation(program, "u_UseTexture");
	// textures
	char name[50];
	for (int i=0; i<kMaxTextureUnit; i++) {
		std::sprintf(name, "u_TextureMatrix[%d]", i);
		uniformTextureMatrix[i] = glGetUniformLocation(program, name);
		std::sprintf(name, "u_Texture[%d]", i);
		uniformTextures[i] = glGetUniformLocation(program, name);
	}
	// lights
	for (int i=0; i<kMaxLightUnit; i++) {
		std::sprintf(name, "u_LightSource[%d].ambient", i);
		uniformLights[i][UNIFORM_LIGHT_AMBIENT] = glGetUniformLocation(program, name);
		std::sprintf(name, "u_LightSource[%d].diffuse", i);
		uniformLights[i][UNIFORM_LIGHT_DIFFUSE] = glGetUniformLocation(program, name);
		std::sprintf(name, "u_LightSource[%d].specular", i);
		uniformLights[i][UNIFORM_LIGHT_SPECULAR] = glGetUniformLocation(program, name);
		std::sprintf(name, "u_LightSource[%d].position", i);
		uniformLights[i][UNIFORM_LIGHT_POS] = glGetUniformLocation(program, name);
	}
	uniforms[UNIFORM_MAX_LIGHT] = glGetUniformLocation(program, "u_MaxLights");

	this->prepareExtraShader(program);
}

void BaseShader::setInfo(Scene *scene, Figure *figure, Camera *camera) {
	if (!figure || !camera) return;
	Matrix3D mvp(camera->projectionMatrix);
	
	// viewMatrix
	camera->updateViewMatrix();
	mvp.multiply(&camera->viewMatrix);
	
	// modelMatrix
	mvp.multiply(&figure->globalMatrix);
	
	if (uniforms[UNIFORM_MV_MATRIX]>=0) {
		glUniformMatrix4fv(uniforms[UNIFORM_MV_MATRIX], 1, GL_FALSE, figure->globalMatrix.matrix);
	}
	if (uniforms[UNIFORM_P_MATRIX]>=0) {
		glUniformMatrix4fv(uniforms[UNIFORM_P_MATRIX], 1, GL_FALSE, camera->projectionMatrix.matrix);
	}
	if (uniforms[UNIFORM_MVP_MATRIX]>=0) {
		glUniformMatrix4fv(uniforms[UNIFORM_MVP_MATRIX], 1, GL_FALSE, mvp.matrix);
	}
	
	// normal matrix
	if (uniforms[UNIFORM_NORMAL_MATRIX]>=0) {
		GLfloat normalMtx[9];
		figure->globalMatrix.normalMatrix(normalMtx);
		glUniformMatrix3fv(uniforms[UNIFORM_NORMAL_MATRIX], 1, GL_FALSE, normalMtx);
	}
	
	// texture
	glBindTexture(GL_TEXTURE_2D, 0);
	if (figure->material) {
		// single
		Texture_ptr tex[] = {figure->material->texture0, figure->material->texture1, figure->material->texture2};
		if (tex[0] && uniforms[UNIFORM_TEXTURE]>=0) {
			tex[0]->bind();
			glActiveTexture(GL_TEXTURE0);
			glUniform1i(uniforms[UNIFORM_TEXTURE], 0);
			if (uniforms[UNIFORM_TEX_MATRIX]>=0) {
				glUniformMatrix4fv(uniforms[UNIFORM_TEX_MATRIX], 1, GL_FALSE, tex[0]->matrix.matrix);
			}
			glUniform1i(uniforms[UNIFORM_USE_TEXTURE], 1);
		} else {
			// multi
			bool useflg = false;
			for (int i=0; i<kMaxTextureUnit; i++) {
				if (tex[i] && uniformTextures[i]>=0) {
					tex[i]->bind();
					glActiveTexture(GL_TEXTURE0+i);
					glUniform1i(uniformTextures[i], i);
					if (uniformTextureMatrix[i]>=0) {
						glUniformMatrix4fv(uniformTextureMatrix[i], 1, GL_FALSE, tex[i]->matrix.matrix);
					}
					useflg = true;
				}
			}
			if (useflg) {
				glUniform1i(uniforms[UNIFORM_USE_TEXTURE], 1);
			} else {
				glUniform1i(uniforms[UNIFORM_USE_TEXTURE], 0);
			}
		}
	} else {
		glUniform1i(uniforms[UNIFORM_USE_TEXTURE], 0);
	}

	// lights
	std::vector<Node*> lights = scene->getLights();
	int count = 0;
	GLfloat lightParam[4];
	for (int i=0; i<kMaxLightUnit; i++) {
		if (lights.size()>i) {
			Light *lt = (Light*)lights[i];
			// pos
			lightParam[0] = lt->globalMatrix.getX();
			lightParam[1] = lt->globalMatrix.getY();
			lightParam[2] = lt->globalMatrix.getZ();
			lightParam[3] = 1;
			glUniform4fv(uniformLights[i][UNIFORM_LIGHT_POS], 1, lightParam);
			// ambient
			lightParam[0] = lt->ambient.r;
			lightParam[1] = lt->ambient.g;
			lightParam[2] = lt->ambient.b;
			lightParam[3] = lt->ambient.a;
			glUniform4fv(uniformLights[i][UNIFORM_LIGHT_AMBIENT], 1, lightParam);
			// diffuse
			lightParam[0] = lt->diffuse.r;
			lightParam[1] = lt->diffuse.g;
			lightParam[2] = lt->diffuse.b;
			lightParam[3] = lt->diffuse.a;
			glUniform4fv(uniformLights[i][UNIFORM_LIGHT_DIFFUSE], 1, lightParam);
			// specular
			lightParam[0] = lt->specular.r;
			lightParam[1] = lt->specular.g;
			lightParam[2] = lt->specular.b;
			lightParam[3] = lt->specular.a;
			glUniform4fv(uniformLights[i][UNIFORM_LIGHT_SPECULAR], 1, lightParam);
			count++;
		}
	}
	glUniform1i(uniforms[UNIFORM_MAX_LIGHT], count);

	// extra
	this->setExtraInfo(figure, camera);
}


