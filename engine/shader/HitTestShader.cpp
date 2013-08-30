//
//  HitTestShader.cpp
//  GCube
//
//  Created by Takashi Tsuchiya on 2013/08/29.
//  Copyright (c) 2013年 GClue, Inc. All rights reserved.
//

#include "HitTestShader.h"
#include "../scene/Mesh.h"
#include "../scene/Camera.h"
#include "../scene/Figure.h"
#include "../util/Log.h"

using namespace GCube;

HitTestShader::HitTestShader() {
	this->reload();
	this->reset();
}

void HitTestShader::setInfo(Figure *figure, Camera *camera) {
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
	// 色作成
	count++;
	int r = count >> 16 & 0xff;
	int g = count >>  8 & 0xff;
	int b = count       & 0xff;
	Colorf color(r/255.0, g/255.0, b/255.0, 1);
	//LOGD("%d, %d, %d", r, g, b);
	glUniform4f(uniforms[UNIFORM_COLOR], color.r, color.g, color.b, color.a);
	figure->testColor = color;
}

void HitTestShader::reset() {
	count = 0;
}
