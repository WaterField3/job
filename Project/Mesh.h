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
	std::vector<D3D::VERTEX_3D>		m_vertices;		// ���_���W�Q
	std::vector<unsigned int>	m_indices;		// �C���f�b�N�X�f�[�^�Q
public:
	// ���_�f�[�^�擾
	const std::vector<D3D::VERTEX_3D>& GetVertices() {
		return m_vertices;
	}

	// �C���f�b�N�X�f�[�^�擾
	const std::vector<unsigned int>& GetIndices() {
		return m_indices;
	}
	};
}

