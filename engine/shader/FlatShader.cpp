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

//////////////////////////////////////////////////////////
// vertex shader
CONST_STR(gVertexShader,

struct LightSourceParameters {
  vec4 ambient; // Acli
  vec4 diffuse; // Dcli
  vec4 specular; // Scli
  vec4 position; // Ppli
};

struct MaterialParameters
{
	vec4 emission;    // Ecm
	vec4 ambient;     // Acm
	vec4 diffuse;     // Dcm
	vec4 specular;    // Scm
	float shininess;  // Srm
};

const int ZERO = 0;
const int ONE = 1;

uniform mat4 u_ModelViewMatrix;
uniform mat4 u_ModelViewProjectionMatrix;
uniform mat3 u_NormalMatrix;
uniform mat4 u_TextureMatrix;
uniform int u_MaxLights;
uniform LightSourceParameters u_LightSource[5];
uniform bool u_UseColor;
uniform MaterialParameters u_FrontMaterial;
uniform bool u_UseMaterial;
attribute vec3 a_Vertex;
attribute vec3 a_Normal;
attribute vec3 a_Color;
attribute vec2 a_MultiTexCoord0;
varying vec2 v_Texcoord;
varying vec4 v_Color;

void main()
{
	// light
	vec4 pos = u_ModelViewMatrix * vec4(a_Vertex, ONE);
	vec3 normal = normalize(u_NormalMatrix * a_Normal);
	v_Color = vec4(ZERO);
	for( int i = ZERO; i < u_MaxLights; i++ ) {
		if (i==ZERO) v_Color = vec4(ZERO);
		vec3 lightDir = normalize(u_LightSource[i].position.xyz - vec3(pos));
		if (u_UseMaterial) {
			v_Color += dot(lightDir, normal) * u_LightSource[i].diffuse * u_FrontMaterial.diffuse;
		} else {
			v_Color += dot(lightDir, normal) * u_LightSource[i].diffuse;
		}
	}
	// VertexColor
	if (u_UseColor) {
		v_Color *= vec4(a_Color, ONE);
	}
	// Material
	if (u_UseMaterial) {
		v_Color += u_FrontMaterial.ambient;
	}
	// tex
    v_Texcoord = vec2(u_TextureMatrix * vec4(a_MultiTexCoord0, ZERO, ONE));
	// pos
	gl_Position = u_ModelViewProjectionMatrix * vec4(a_Vertex, ONE);
}

);

// fragment shader
CONST_STR(gFragmentShader,

precision mediump float;
uniform sampler2D u_Texture;
uniform bool u_UseTexture;
varying vec2 v_Texcoord;
varying vec4 v_Color;

void main()
{
	if (u_UseTexture) {
		gl_FragColor = vec4(texture2D(u_Texture, vec2(v_Texcoord.st))) * v_Color;
	} else {
		gl_FragColor = v_Color;
	}
}

);

//////////////////////////////////////////////////////////


FlatShader::FlatShader() {
	this->reload();
}

void FlatShader::reload() {
	gProgram = loadShader(gVertexShader, gFragmentShader);
}
