#pragma once

#include <vector>

#include "direct3d.h"

class Mesh
{
public:
	std::vector<D3D::Vertex>  m_vertices;
	std::vector<unsigned int> m_indices;
protected:
	// 頂点データ取得
	const std::vector<D3D::Vertex>& GetVertices();
	// インデックスデータ取得
	const std::vector<unsigned int>& GetIndices();

};

