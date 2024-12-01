#include "MeshRenderer.h"

namespace TMF
{
	void MeshRenderer::Init(Mesh& mesh)
	{
		m_VertexBuffer.Create(mesh.GetVertices());
		m_IndexBuffer.Create(mesh.GetIndices());
		m_IndexNum = static_cast<int>(mesh.GetIndices().size());
	}
	void MeshRenderer::BeforeDraw()
	{

		auto devicecontext = D3D::Get()->GetContext();

		// �g�|���W�[���Z�b�g�i���v���~�e�B�u�^�C�v�j
		devicecontext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		m_VertexBuffer.SetGPU();			// ���_�o�b�t�@���Z�b�g
		m_IndexBuffer.SetGPU();
	}
	void MeshRenderer::DrawSubset(unsigned int indexnum, unsigned int baseindex, unsigned int basevertexindex)
	{
		D3D::Get()->GetContext()->DrawIndexed(
			indexnum,								// �`�悷��C���f�b�N�X���i�ʐ��~�R�j
			baseindex,								// �ŏ��̃C���f�b�N�X�o�b�t�@�̈ʒu
			basevertexindex);						// ���_�o�b�t�@�̍ŏ�����g��
	}
	void MeshRenderer::Draw()
	{
		BeforeDraw();								// �`��O����

		D3D::Get()->GetContext()->DrawIndexed(
			m_IndexNum,								// �`�悷��C���f�b�N�X���i�ʐ��~�R�j
			0,										// �ŏ��̃C���f�b�N�X�o�b�t�@�̈ʒu
			0);										// ���_�o�b�t�@�̍ŏ�����g��
	}
	void MeshRenderer::Modify(const std::vector<D3D::VERTEX_3D>& vertices)
	{
		m_VertexBuffer.Modify(vertices);
	}
}