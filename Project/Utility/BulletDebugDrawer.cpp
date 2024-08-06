#include "BulletDebugDrawer.h"

#include "direct3d.h"

BulletDebugDrawer::BulletDebugDrawer(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, ID3D11InputLayout* pInputLayout)
{
	m_pState = std::make_unique<DirectX::CommonStates>(pDevice);
	m_pBatch = std::make_unique<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>>(pDeviceContext);
}

BulletDebugDrawer::~BulletDebugDrawer()
{
	m_pState.reset();
	m_pBatch.reset();
}

void BulletDebugDrawer::drawLine(const btVector3& from, const btVector3& to, const btVector3& color)
{
	m_vertices.push_back(DirectX::SimpleMath::Vector3(from.x(), from.y(), from.z()));
	m_vertices.push_back(DirectX::SimpleMath::Vector3(to.x(), to.y(), to.z()));
	m_colors.push_back(DirectX::SimpleMath::Vector4(color.x(), color.y(), color.z(), 1.0f));
	m_colors.push_back(DirectX::SimpleMath::Vector4(color.x(), color.y(), color.z(), 1.0f));
}

void BulletDebugDrawer::drawContactPoint(const btVector3& pointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color)
{
	m_vertices.push_back(DirectX::SimpleMath::Vector3(pointOnB.x(), pointOnB.y(), pointOnB.z()));
	m_colors.push_back(DirectX::SimpleMath::Vector4(color.x(), color.y(), color.z(), 1.0f));
}

void BulletDebugDrawer::reportErrorWarning(const char* warningString)
{

}

void BulletDebugDrawer::draw3dText(const btVector3& location, const char* textString)
{

}

void BulletDebugDrawer::setDebugMode(int debugMode)
{
	m_debugMode = debugMode;
}

int BulletDebugDrawer::getDebugMode() const
{
	return m_debugMode;
}

void BulletDebugDrawer::Render(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, ID3D11InputLayout* pInputLayout)
{
	if (m_vertices.empty())
	{
		return;
	}

	// ※頂点バッファ→VRAMに頂点データを置くための機能
	D3D11_BUFFER_DESC bufferDesc;
	bufferDesc.ByteWidth = 3 * m_vertices.size();// 確保するバッファサイズを指定

	pDeviceContext->OMSetBlendState(m_pState->Opaque(), nullptr, 0xFFFFFFFF);
	pDeviceContext->OMSetDepthStencilState(m_pState->DepthNone(), 0);
	pDeviceContext->RSSetState(m_pState->CullNone());
	pDeviceContext->IASetInputLayout(pInputLayout);
	m_pBatch->Begin();

	auto divisions = m_vertices.size();

	for (int i = 0; i <= divisions - 2; ++i)
	{

		DirectX::VertexPositionColor v1(m_vertices[i], DirectX::SimpleMath::Vector4(0, 0, 0, 1));
		DirectX::VertexPositionColor v2(m_vertices[i + 1], DirectX::SimpleMath::Vector4(0, 0, 0, 1));
		m_pBatch->DrawLine(v1, v2);
	}
	m_pBatch->End();
	D3D11_RASTERIZER_DESC rastraizerDesc;
	ZeroMemory(&rastraizerDesc, sizeof(D3D11_RASTERIZER_DESC));
	rastraizerDesc.CullMode = D3D11_CULL_FRONT;
	rastraizerDesc.FillMode = D3D11_FILL_SOLID;

	ID3D11RasterizerState* RasterrizeState;

	pDevice->CreateRasterizerState(&rastraizerDesc, &RasterrizeState);
	pDeviceContext->RSSetState(RasterrizeState);
	m_vertices.clear();
	m_colors.clear();
}
