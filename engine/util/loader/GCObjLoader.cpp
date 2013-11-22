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

#include "../../GCube.h"
#include "GCObjLoader.h"
#include <sstream>

using namespace GCube;

// ファイルから読み込み
Figure_ptr GCObjLoader::loadFile(const char *fileName, bool rightHanded) {
	// リソース読み込み
	ApplicationController *ctr = ApplicationController::SharedInstance();
	std::vector<char> data;
	ctr->getResource(fileName, data);
	LOGI("***GCObjLoader***");
	LOGI("file:%s", fileName);
	return loadData(&data);
}

// データから読み込み
Figure_ptr GCObjLoader::loadData(std::vector<char>* data, bool rightHanded) {
	
	std::istringstream stream(std::string(data->begin(), data->end()));
	char buff[256];
	
	// ヘッダ確認
	stream.read(buff, 6);
	buff[6] = '\0';
	if (strcmp(buff, "gcub02")!=0) {
		LOGE("not a gcube obj file!");
		return NULL;
	}

	// 各セクション読み込み
	Mesh_ptr mesh;
	
	short type = 0;
	stream.read((char*)&type, sizeof(short));
	LOGI("type:%d", type);
	switch (type) {
		case 0:
			mesh = meshProcess(stream);
			break;
			
		default:
			LOGE("unknown type [%d]", type);
			exit(-1);
			break;
	}
		
	Figure_ptr fig = Figure_ptr(new Figure("Fig"));
	fig->mesh = mesh;
	//	// マテリアルとシェーダー設定
	//	fig->material = Material_ptr(new Material());
	//	fig->material->texture = Texture_ptr(new Texture("texture/gclue_logo.png"));
	//	fig->material->ambientColor = Colorf(0.5, 0, 0);
	
	return fig;
}

Mesh_ptr GCObjLoader::meshProcess(std::istringstream &stream) {
	char buff[512];
	short len = 0;
	
	// header
	stream.read((char*)&len, sizeof(short));
	LOGI("len:%d", len);
	stream.read(buff, len);
	buff[len] = '\0';
	LOGI("str:%s", buff);
	bool usecolor = false;
	stream.read((char*)&usecolor, sizeof(bool));
	LOGI("usecolor:%d", usecolor);
	short uvnum = 0;
	stream.read((char*)&uvnum, sizeof(short));
	LOGI("uvnum:%d", uvnum);
	int vnum = 0;
	stream.read((char*)&vnum, sizeof(int));
	LOGI("vnum:%d", vnum);
	
	int size = vnum * (6 + (usecolor?3:0) + uvnum * 2) * sizeof(float);
	LOGI("size:%d", size);
	
	// data
	// TODO: バッファオーバー対策
	stream.read(buff, size);
	float *f_buf = (float*)buff;
	std::vector<float> interleaveArray(f_buf, f_buf+size);
	
	//3,3,3,2*n
	std::vector<AttribType> attribArray;
	attribArray.push_back(AttribTypeVertex);
	attribArray.push_back(AttribTypeNormal);
	if (usecolor) {
		attribArray.push_back(AttribTypeColor);
	}
	for (int i=0; i<uvnum; i++) {
		attribArray.push_back((AttribType)(AttribTypeUV+i));
	}
	
	// index
	std::vector<short> indexArray;
	for (short i=0; i<vnum; i++) {
		indexArray.push_back(i);
	}
	
	// make
	MeshInterleaveData_ptr mdata(new MeshInterleaveData());
	mdata->data = interleaveArray;
	mdata->vertexIndexes = indexArray;
	mdata->attribs = attribArray;
	
	Mesh_ptr mesh(new Mesh());
	mesh->build(mdata);
	
	return mesh;
}
