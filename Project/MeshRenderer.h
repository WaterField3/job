#pragma once

#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Mesh.h"

namespace TMF
{
	class MeshRenderer
	{
protected:
	VertexBuffer<D3D::VERTEX_3D>	m_VertexBuffer;		// ���_�o�b�t�@
	IndexBuffer				m_IndexBuffer;		// �C���f�b�N�X�o�b�t�@
	int							m_IndexNum = 0;		// �C���f�b�N�X��
public:
	virtual void Init(Mesh& mesh);

	// �`��O����
	virtual void BeforeDraw();

	virtual void DrawSubset(unsigned int indexnum, unsigned int baseindex, unsigned int basevertexindex);

	virtual void Draw();

	void Modify(const std::vector<D3D::VERTEX_3D>& vertices);

	};
}

