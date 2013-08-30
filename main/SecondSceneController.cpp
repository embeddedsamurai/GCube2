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

#include "SecondSceneController.h"


using namespace GCube;

// 初期化
void SecondSceneController::onInit() {
	LOGD("SecondSceneController::onInit()");
	
	ApplicationController *ctr = ApplicationController::SharedInstance();
	
	// 音読み込み
	int bgmid = SoundPlayer::loadBGM("sound/bgm_house.ogg");
	SoundPlayer *player = SoundPlayer::SharedInstance();
	player->play(bgmid);
	sid = SoundPlayer::loadSE("sound/se_yay.ogg");
	
	// シーン作成
	Scene_ptr sceneMain(new Scene("SceneMain"));
	Node_ptr node(new Node("Node"));
	node->transform.translate(3, 3, 0);
	// プレート追加
	fig = Figure_ptr(new Figure("Fig"));
	fig->mesh = PrimitiveObject::createPlate(Sizef(5, 3));
	// マテリアルとシェーダー設定
	fig->material = Material_ptr(new Material());
	fig->material->texture = Texture_ptr(new Texture("texture/gclue_logo.png"));
	fig->material->ambientColor = Colorf(0, 0.5, 0.5);
	fig->shader = ShaderManager::GetShader(ShaderTypeTex);
	fig->transform.translate(0, 3, 0);
	fig->touchEventListener = this;
	fig->isTouchable = true;
	node->addChildNode(fig);
	// プレート追加２
	fig2 = Figure_ptr(new Figure("Fig2"));
	fig2->mesh = PrimitiveObject::createBox(Point3f(2, 3, 4));
	fig2->material = Material_ptr(new Material());
	fig2->material->ambientColor = Colorf(0, 0.5, 0.5);
	fig2->shader = ShaderManager::GetShader(ShaderTypeFlat);
	fig2->transform.translate(3, 1, 0);
	fig2->transform.rotate(45, RotateDirX);
	fig2->transform.rotate(45, RotateDirY);
	fig2->touchEventListener = this;
	fig2->isTouchable = true;
	node->addChildNode(fig2);
	
	// ライト
	Light_ptr light(new Light());
	light->transform.translate(-3, 2, 9);
	light->diffuse = Colorf(0.5,0,0);
	node->addChildNode(light);
	Light_ptr light2(new Light());
	light2->transform.translate(6, 2, 9);
	node->addChildNode(light2);
	
	// シーン変更
	sceneMain->addChildNode(node);
	ctr->sceneMap[2] = sceneMain;
	
	// メインウィンドウの背景色変更
	Window_ptr mainWindow = ctr->windowArray[0];
	mainWindow->bgColor = Colorf(0,1,0);
	
	// サブウィンドウ作成
	Window_ptr window(new Window());
	window->frame = Rectf(0.7, 0.7, 0.5, 0.5);
	window->bgColor = Colorf(1,0,0);
	// サブウィンドウにカメラ設定
	subCamera = StandardCamera_ptr(new StandardCamera("CameraMain"));
	sceneMain->addChildNode(subCamera);
	subCamera->transform.loadIdentity();
	subCamera->transform.translate(0, 0, 40);
	subCamera->transform.rotate(45, GCube::RotateDirZ);
	window->camera = subCamera;
	window->isVisible = false;
	ctr->windowArray.push_back(window);
	
}

// 更新
static float ang = 0;
void SecondSceneController::onUpdate(float dt) {
	// サブウィンドウのカメラを回転
	subCamera->transform.loadIdentity();
	subCamera->transform.translate(0, 0, 40);
	ang += 3;
	subCamera->transform.rotate(ang, GCube::RotateDirZ);
	fig->material->texture->matrix.rotate(3, RotateDirX);
}

// ノードタッチイベント
static Pointf pos;
static Matrix3D mtx;
void SecondSceneController::onTouchNode(TouchableNode& node, const TouchEvent &event) {
	//LOGD("touch![%s]", node.name);
	if (event.action == TouchActionDown) {
		pos = event.pos;
		mtx = node.transform;
		// 音再生
		SoundPlayer *player = SoundPlayer::SharedInstance();
		player->play(sid);
	} else {
		float dx = event.pos.x - pos.x;
		float dy = event.pos.y - pos.y;
		node.transform.loadIdentity();
		node.transform.translate(dx * 13.0, dy * 13.0, 0);
		node.transform.multiply(&mtx);
	}
}

