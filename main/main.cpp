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
//#include "glsample.h"

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
	// プレート追加
	Mesh_ptr mesh = PrimitiveObject::createPlate(Sizef(5, 3));
	Figure_ptr fig(new Figure("Fig"));
	fig->shader = SimpleShader_ptr(new SimpleShader());
	fig->mesh = mesh;
	sceneMain->rootNode.addChildNode(fig);
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
	sceneMain->rootNode.addChildNode(subCamera);
	subCamera->transform.loadIdentity();
	subCamera->transform.translate(0, 0, 40);
	subCamera->transform.rotate(45, GCube::RotateDirZ);
	window->camera = subCamera;
	ctr->windowArray.push_back(window);

	//
	this->onContextChanged();
}

// サイズ変更
void Main::onSizeChanged(float width, float height, DeviceOrientation orientation) {
	LOGD("Main::onSizeChanged(%f, %f, %d)", width, height, orientation);
}

// コンテキスト切り替え
void Main::onContextChanged() {
	// リソース読み込み
	ApplicationController *ctr = ApplicationController::SharedInstance();
	std::vector<char> buf;
	ctr->getResource("texture/gclue_logo.png", buf);
}

// 更新
static float ang = 0;
void Main::onUpdate(float dt) {
	// サブウィンドウのカメラを回転
	subCamera->transform.loadIdentity();
	subCamera->transform.translate(0, 0, 40);
	ang += 3;
	subCamera->transform.rotate(ang, GCube::RotateDirZ);
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
