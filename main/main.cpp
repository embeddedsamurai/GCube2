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

#include "Main.h"

using namespace GCube;

// 初期化
void Main::onInit() {
	LOGD("Main::onInit()");
	
	ApplicationController *ctr = ApplicationController::SharedInstance();
	// 言語設定を取得
	LOGD("lang:%s", ctr->getLanguage().c_str());
	// UserEventを送信
	LOGD("send user event:%d", ctr->sendUserEvent(1, 2, 3, 4, 5, "a"));
	
	// ファイル書き込み
	std::string filePath = ctr->getStoragePath(StorageTypeDocument);
	filePath += "hello.txt";
	LOGD("file:%s", filePath.c_str());
	FILE* file = fopen(filePath.c_str(), "w+");
	if (file != NULL) {
		fputs("HELLO WORLD!\n", file);
		fflush(file);
		fclose(file);
	}
	
	// 音読み込み
	int bgmid = SoundPlayer::loadBGM("sound/bgm_house.ogg");
	SoundPlayer *player = SoundPlayer::SharedInstance();
	player->play(bgmid);
	sid = SoundPlayer::loadSE("sound/se_yay.ogg");
	
	// シーン作成
	Scene_ptr sceneMain(new Scene("SceneMain"));
	Node_ptr node(new Node("Node"));
	node->transform.translate(2, 0, 0);
//	node->transform.rotate(90, RotateDirZ);
	// プレート追加
	fig = Figure_ptr(new Figure("Fig"));
	fig->mesh = PrimitiveObject::createPlate(Sizef(5, 3));
	// マテリアルとシェーダー設定
	fig->material = Material_ptr(new Material());
	fig->material->texture = Texture_ptr(new Texture("texture/gclue_logo.png"));
	fig->shader = TexShader_ptr(new TexShader());
	node->addChildNode(fig);
	// プレート追加２
	fig2 = Figure_ptr(new Figure("Fig2"));
//	fig2->mesh = PrimitiveObject::createPlate(Sizef(2, 3));
	fig2->mesh = PrimitiveObject::createBox(Point3f(2, 3, 4));
	fig2->material = Material_ptr(new Material());
	fig2->material->ambientColor = Colorf(0, 0.5, 0.5);
//	fig2->material->texture = Texture_ptr(new Texture("texture/gclue_logo.png"));
//	fig2->shader = ColorShader_ptr(new ColorShader());
	fig2->shader = FlatShader_ptr(new FlatShader());
//	fig2->shader = ShaderManager::GetShader(ShaderTypeColor);
	fig2->transform.translate(3, 1, 0);
	fig2->transform.rotate(45, RotateDirX);
	fig2->transform.rotate(45, RotateDirY);
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
	ctr->changeScene(sceneMain);

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
	ctr->windowArray.push_back(window);
	
}

// サイズ変更
void Main::onSizeChanged(float width, float height, DeviceOrientation orientation) {
	LOGD("Main::onSizeChanged(%f, %f, %d)", width, height, orientation);
}

// コンテキスト切り替え
void Main::onContextChanged() {
	LOGD("Main::onContextChanged()");
	
	// Figure再構築
	fig->rebuild();
	fig2->rebuild();
}

// 更新
static float ang = 0;
void Main::onUpdate(float dt) {
	// サブウィンドウのカメラを回転
	subCamera->transform.loadIdentity();
	subCamera->transform.translate(0, 0, 40);
	ang += 3;
	subCamera->transform.rotate(ang, GCube::RotateDirZ);
	fig->material->texture->matrix.rotate(3, RotateDirX);
	
	fig2->transform.loadIdentity();
	fig2->transform.translate(3, 1, 0);
	fig2->transform.rotate(45, RotateDirX);
	fig2->transform.rotate(ang, RotateDirY);

//	tex->matrix.scale(1.01, 1.01, 1.01);
}

// タッチイベント
void Main::onTouch(TouchAction action, float x, float y, long id, long time) {
	if (action==TouchActionDown) {
		// 音再生
		SoundPlayer *player = SoundPlayer::SharedInstance();
		player->play(sid);
	}
}

// ユーザイベント
int Main::onUserEvent(int type, int param1, long long param2, float param3, double param4, const char *param5) {
	LOGD("Main::onUserEvent(t: %d, 1: %d, 2: %lld, 3:%f, 4:%f, 5:%s)", type, param1, param2, param3, param4, param5);
	return type;
}

// デバッグコマンド
void Main::onDebugCommand(const char *command, int param) {
	LOGD("onDebugCommand(%s, %d)", command, param);
	if (strcmp(command, "test")==0) {
		LOGD("***TestCommand (%d)", param);
	}
}
