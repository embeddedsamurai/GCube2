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

#ifndef __GCube__Mesh__
#define __GCube__Mesh__

#include "GCDefines.h"
#include "Vbo.h"

namespace GCube {

// attribute index
enum {
	ATTRIB_VERTEX,
	ATTRIB_NORMAL,
	ATTRIB_TEXCOORD,
	ATTRIB_TEXCOORD_MLT,
	ATTRIB_INDEX,
	ATTRIB_JOINTS,
	NUM_ATTRIBUTES
};

// data
struct MeshData {
	std::vector<float> vertices;		//!< 頂点データ.
	std::vector<float> normals;			//!< 法線データ.
	std::vector<float> textureCoords;	//!< uvデータ.
	std::vector<float> mltTextureCoords;//!< uvデータ（マルチテクスチャ用）.
	std::vector<float> jointData;		//!< jointデータ.（内容はjoint1, weight1, joint2, weight2の順）
	std::vector<short> vertexIndexes;	//!< 頂点インデックスデータ.
	
	/**
	 * 頂点座標を追加します.
	 * @param[in] v 追加する頂点
	 * @param[in] len 追加する頂点の個数
	 */
	void addVertices(const float *v, int len);
	
	/**
	 * 法線ベクトルを追加します.
	 * @param[in] v 追加する法線ベクトル
	 * @param[in] len 追加する法線ベクトルの個数
	 */
	void addNormal(const float *v, int len);
	
	/**
	 * uvを追加します.
	 * @param[in] v 追加するuv
	 * @param[in] len 追加するuvの個数
	 */
	void addTextureCoords(const float *v, int len, bool mlt=false);
	
	/**
	 * Jointを追加します.
	 * @param[in] j1 １番目jointインデックス
	 * @param[in] w1 １番目Weight
	 * @param[in] j2 ２番目jointインデックス
	 * @param[in] w2 ２番目Weight
	 * @param[in] len Joint個数
	 */
	void addJoints(const unsigned short *j1, const float *w1, const unsigned short *j2, const float *w2, int len);
	
	/**
	 * インデックスを追加します.
	 * @param[in] v 追加するインデックス
	 * @param[in] len 追加するインデックスの個数
	 */
	void addVertexIndexes(const unsigned short *v, int len);
};
DEF_SHARED_PTR(MeshData);


class Mesh {
public:
	Mesh();
	virtual ~Mesh() {};
	
	virtual void build(const MeshData_ptr &data);
	virtual void rebuild();
	virtual void bind();
	virtual int getIndexCount();
	

private:
	int indexCount;
	Vbo_ptr vbo;
	
};
DEF_SHARED_PTR(Mesh);
}

#endif /* defined(__GCube__Mesh__) */
