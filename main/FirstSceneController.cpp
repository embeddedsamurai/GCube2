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

#include "FirstSceneController.h"

using namespace GCube;

// 初期化
void FirstSceneController::onInit() {
	LOGD("FirstSceneController::onInit()");
	
	ApplicationController *ctr = ApplicationController::SharedInstance();
	
	// 音読み込み
	int bgmid = SoundPlayer::loadBGM("sound/bgm_house.ogg");
	SoundPlayer *player = SoundPlayer::SharedInstance();
	player->play(bgmid);
	sid = SoundPlayer::loadSE("sound/se_yay.ogg");
	
	// シーン作成
	Scene_ptr sceneMain(new Scene("SceneFirst"));
	// プレート追加
	Figure_ptr fig;
	fig = Figure_ptr(new Figure("Fig"));
	fig->mesh = PrimitiveObject::createPlate(Sizef(5, 5));
	// マテリアルとシェーダー設定
	fig->material = Material_ptr(new Material());
//	fig->material->texture = Texture_ptr(new Texture("texture/gclue_logo.png"));
	fig->material->ambientColor = Colorf(0, 0, 0.5);
	fig->shader = ShaderManager::GetShader(ShaderTypeColor);
	fig->transform.translate(0, 3, 0);
	fig->touchEventListener = this;
	fig->isTouchable = true;
	
	// シーン変更
	sceneMain->addChildNode(fig);
	ctr->sceneMap[1] = sceneMain;
	ctr->changeScene(1);
}

// ノードタッチイベント
void FirstSceneController::onTouchNode(TouchableNode& node, const TouchEvent &event) {
	if (event.action == TouchActionDown) {
		// 音再生
		SoundPlayer *player = SoundPlayer::SharedInstance();
		player->play(sid);
		// シーン遷移
		ApplicationController *ctr = ApplicationController::SharedInstance();
		ctr->changeScene(2);
		// サブウィンドウ表示
		Window_ptr mainWindow = ctr->windowArray[1];
		mainWindow->isVisible = true;
	}
}

