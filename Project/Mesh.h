#pragma once

#include <vector>
#include "direct3d.h"

namespace TMF
{
	class Mesh
	{
	public:
		Mesh() {};
		~Mesh() = default;
	protected:
	std::vector<D3D::VERTEX_3D>		m_vertices;		// 頂点座標群
	std::vector<unsigned int>	m_indices;		// インデックスデータ群
public:
	// 頂点データ取得
	const std::vector<D3D::VERTEX_3D>& GetVertices() {
		return m_vertices;
	}

	// インデックスデータ取得
	const std::vector<unsigned int>& GetIndices() {
		return m_indices;
	}
	};
}

