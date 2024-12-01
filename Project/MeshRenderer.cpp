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

		// トポロジーをセット（旧プリミティブタイプ）
		devicecontext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		m_VertexBuffer.SetGPU();			// 頂点バッファをセット
		m_IndexBuffer.SetGPU();
	}
	void MeshRenderer::DrawSubset(unsigned int indexnum, unsigned int baseindex, unsigned int basevertexindex)
	{
		D3D::Get()->GetContext()->DrawIndexed(
			indexnum,								// 描画するインデックス数（面数×３）
			baseindex,								// 最初のインデックスバッファの位置
			basevertexindex);						// 頂点バッファの最初から使う
	}
	void MeshRenderer::Draw()
	{
		BeforeDraw();								// 描画前処理

		D3D::Get()->GetContext()->DrawIndexed(
			m_IndexNum,								// 描画するインデックス数（面数×３）
			0,										// 最初のインデックスバッファの位置
			0);										// 頂点バッファの最初から使う
	}
	void MeshRenderer::Modify(const std::vector<D3D::VERTEX_3D>& vertices)
	{
		m_VertexBuffer.Modify(vertices);
	}
}