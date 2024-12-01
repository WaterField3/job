#pragma once

#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Mesh.h"

namespace TMF
{
	class MeshRenderer
	{
protected:
	VertexBuffer<D3D::VERTEX_3D>	m_VertexBuffer;		// 頂点バッファ
	IndexBuffer				m_IndexBuffer;		// インデックスバッファ
	int							m_IndexNum = 0;		// インデックス数
public:
	virtual void Init(Mesh& mesh);

	// 描画前処理
	virtual void BeforeDraw();

	virtual void DrawSubset(unsigned int indexnum, unsigned int baseindex, unsigned int basevertexindex);

	virtual void Draw();

	void Modify(const std::vector<D3D::VERTEX_3D>& vertices);

	};
}

