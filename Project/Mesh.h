#pragma once

#include <vector>

#include "direct3d.h"

class Mesh
{
public:
	std::vector<D3D::Vertex>  m_vertices;
	std::vector<unsigned int> m_indices;
protected:
	// ���_�f�[�^�擾
	const std::vector<D3D::Vertex>& GetVertices();
	// �C���f�b�N�X�f�[�^�擾
	const std::vector<unsigned int>& GetIndices();

};

