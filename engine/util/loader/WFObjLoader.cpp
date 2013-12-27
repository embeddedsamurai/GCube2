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
	
	std::map<std::string, Material_ptr> materialMap;
	Figure_ptr fig = Figure_ptr(new Figure("obj"));
	MeshData_ptr mdata(new MeshData());
	Figure_ptr root = fig;

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
		} else
		if (strncmp(line.c_str(), "g ", 2) == 0) {
		} else
		// グループ
		if (strncmp(line.c_str(), "o ", 2) == 0) {
			if (mdata->vertexIndexes.size()) {
				
				if (root!=fig) {
					root->addChildNode(fig);
				}
				
				Mesh_ptr mesh(new Mesh());
				mesh->build(mdata);
				fig->mesh = mesh;
				// TODO:
				fig->shader = ShaderManager::GetShader(ShaderTypeFlat);
				
				fig = Figure_ptr(new Figure("obj"));
				// TODO:
				fig->shader = ShaderManager::GetShader(ShaderTypeFlat);
				fig->isTouchable = true;
				mdata = MeshData_ptr(new MeshData());
				faceCount = 0;
			}
		} else
		if (strncmp(line.c_str(), "usemtl ", 7) == 0) {
			fig->material = materialMap[line.substr(7).c_str()];
		} else
		if (strncmp(line.c_str(), "mtllib ", 7) == 0) {
			materialMap = loadMaterial(line.substr(7).c_str());
		}
		
	}
	
	if (root!=fig) {
		root->addChildNode(fig);
	}
	
	Mesh_ptr mesh(new Mesh());
	mesh->build(mdata);
	fig->mesh = mesh;
	// TODO:
	fig->shader = ShaderManager::GetShader(ShaderTypeFlat);

	return root;
}


// マテリアルファイル読み込み
std::map<std::string, Material_ptr> WFObjLoader::loadMaterial(const char *fileName) {
	LOGD("%s", fileName);
	
	// リソース読み込み
	ApplicationController *ctr = ApplicationController::SharedInstance();
	std::vector<char> data;
	ctr->getResource(fileName, data);
	
	std::map<std::string, Material_ptr> materialMap;
	
	//newmtl 材料名
	//Ns　輝度
	//Ka　環境色
	//Kd　拡散色
	//Ks　反射色
	//Ni　光の屈折率
	//d　 アルファ値
	//illum 0:照明なし、1:反射ハイライトなし、2:Ksの値で反射ハイライトあり
	//map_Kd テクスチャファイル名
	
	std::istringstream stream(std::string(data.begin(), data.end()));
	std::string line;
	Material_ptr material(new Material());
	while(std::getline(stream, line)) {
		// 改行コードを削除
		if (line[line.size()-1] == '\n') line.erase(line.size()-1);
		if (line[line.size()-1] == '\r') line.erase(line.size()-1);
		
		// 材料名
		if (strncmp(line.c_str(), "newmtl ", 7) == 0) {
			material = Material_ptr(new Material());
			materialMap[line.substr(7)] = material;
		} else
		// 環境色
		if (strncmp(line.c_str(), "Ka ", 3) == 0) {
			std::vector<float> values;
			WFObjLoader::scanLine(line, values, 3, true);
			material->ambientColor = Colorf(values[0], values[1], values[2]);
		}
		// 拡散色
		if (strncmp(line.c_str(), "Kd ", 3) == 0) {
			std::vector<float> values;
			WFObjLoader::scanLine(line, values, 3, true);
			material->diffuseColor = Colorf(values[0], values[1], values[2]);
		}
		// 反射色
		if (strncmp(line.c_str(), "Ks ", 3) == 0) {
			std::vector<float> values;
			WFObjLoader::scanLine(line, values, 3, true);
			material->specularColor = Colorf(values[0], values[1], values[2]);
		}
		// テクスチャ
		if (strncmp(line.c_str(), "map_Kd ", 3) == 0) {
			material->texture0 = Texture_ptr(new Texture(line.substr(7).c_str()));
		}
	}
	
	return materialMap;
}


