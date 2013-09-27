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
#include "WFObjLoader.h"
#include <vector>
#include <string>
#include <iterator>
#include <sstream>

using namespace GCube;

// 文字列分解
std::vector<std::string>& WFObjLoader::split(const std::string &s, char delim, std::vector<std::string> &elems) {
    std::stringstream ss(s);
    std::string item;
    while(std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}

// v/vn/vt用の行分割
void WFObjLoader::scanLine(const std::string &line, std::vector<float> &outupt, int max, bool rightHanded) {
	std::vector<std::string> value;
	WFObjLoader::split(line, ' ', value);
	if (value.size() > max) {
		for (int i=0; i < max; i++) {
			float f = atof(value[i+1].c_str());
			if (!rightHanded && i==2) f = -f;
			outupt.push_back(f);
			//LOGD("%s", value[i].c_str());
		}
	}
}

// ファイルから読み込み
Figure_ptr WFObjLoader::loadFile(const char *fileName, bool rightHanded) {
	// リソース読み込み
	ApplicationController *ctr = ApplicationController::SharedInstance();
	std::vector<char> data;
	ctr->getResource(fileName, data);
	return loadData(&data);
}

// データから読み込み
Figure_ptr WFObjLoader::loadData(std::vector<char>* data, bool rightHanded) {
	
	std::vector<float> vertices;		//!< 頂点
	std::vector<float> normals;			//!< 法線
	std::vector<float> textureCoords;	//!< テクスチャ座標
	std::istringstream stream(std::string(data->begin(), data->end()));
	
	MeshData_ptr mdata(new MeshData());

	// １行毎に処理
	unsigned short faceCount = 0;
	std::string line;
	while(std::getline(stream, line)) {
		//LOGD("line:%s", line.c_str());
		// 改行コードを削除
		if (line[line.size()-1] == '\n') line.erase(line.size()-1);
		if (line[line.size()-1] == '\r') line.erase(line.size()-1);
		
		// ポリゴン
		if (strncmp(line.c_str(), "f ", 2) == 0) {
			// ' 'で分解
			std::vector<std::string> value;
			WFObjLoader::split(line, ' ', value);
			if (value.size() > 3) {
				for (int i=0; i < 3; i++) {
					// '/'で分解
					std::vector<std::string> face;
					WFObjLoader::split(value[i+1], '/', face);
					if (value.size() > 3) {
						for (int j=0; j < 3; j++) {
							//LOGD("%s", face[i].c_str());
							// インデックスは１スタートなので、１引く
							int idx = atoi(face[j].c_str()) -1;
							switch (j) {
								case 0:
									// 頂点座標
									//LOGD("ff:%d,%f,%f,%f", idx, vertices[idx*3], vertices.at(idx*3+1), vertices.at(idx*3+2));
									mdata->addVertices(&vertices[idx*3], 3);
									break;
								case 1:
									// テクスチャ座標
									if(idx<0) continue;
									mdata->addTextureCoords(&textureCoords[idx*2], 2);
									break;
								case 2:
									// 法線ベクトル
									if(idx<0) continue;
									mdata->addNormal(&normals[idx*3], 3);
									break;
							}
						}
						// インデックス作成
						mdata->addVertexIndexes(&faceCount, 1);
						faceCount++;
					}
				}
			}
		} else
		// 頂点
		if (strncmp(line.c_str(), "v ", 2) == 0) {
			WFObjLoader::scanLine(line, vertices, 3, rightHanded);
		} else
		// 法線
		if (strncmp(line.c_str(), "vn ", 3) == 0) {
			WFObjLoader::scanLine(line, normals, 3, rightHanded);
		} else
		// テクスチャ
		if (strncmp(line.c_str(), "vt ", 3) == 0) {
			WFObjLoader::scanLine(line, textureCoords, 2, rightHanded);
		}
	}
	
	Mesh_ptr mesh(new Mesh());
	mesh->build(mdata);

	Figure_ptr fig = Figure_ptr(new Figure("Fig"));
	fig->mesh = mesh;
//	// マテリアルとシェーダー設定
//	fig->material = Material_ptr(new Material());
//	fig->material->texture = Texture_ptr(new Texture("texture/gclue_logo.png"));
//	fig->material->ambientColor = Colorf(0.5, 0, 0);

	return fig;
}

