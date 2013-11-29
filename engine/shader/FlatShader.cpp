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

uniform mat4 u_ModelViewMatrix;
uniform mat4 u_ModelViewProjectionMatrix;
uniform mat3 u_NormalMatrix;
uniform mat4 u_TextureMatrix;
uniform int u_MaxLights;
uniform LightSourceParameters u_LightSource[5];
attribute vec3 a_Vertex;
attribute vec3 a_Normal;
attribute vec2 a_MultiTexCoord0;
varying vec2 v_Texcoord;
varying vec4 v_Color;

void main()
{
	// light
	vec4 pos = u_ModelViewMatrix * vec4(a_Vertex, 1.0);
	vec3 normal = normalize(u_NormalMatrix * a_Normal);
	v_Color = vec4(0);
	for( int i = 0; i < u_MaxLights; i++ ) {
		vec3 lightDir = normalize(u_LightSource[i].position.xyz - vec3(pos));
		v_Color += dot(lightDir, normal) * u_LightSource[i].diffuse;
	}
	// tex
    v_Texcoord = vec2(u_TextureMatrix * vec4(a_MultiTexCoord0, 0, 1));
	// pos
	gl_Position = u_ModelViewProjectionMatrix * vec4(a_Vertex, 1.0);
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
