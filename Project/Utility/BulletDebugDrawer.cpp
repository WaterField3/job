#include "BulletDebugDrawer.h"

#include "direct3d.h"

BulletDebugDrawer::BulletDebugDrawer(ID3D11DeviceContext* pDeviceContext)
{
	m_pBatch = std::make_unique<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>>(pDeviceContext);

}

BulletDebugDrawer::~BulletDebugDrawer()
{
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

void BulletDebugDrawer::Render()
{
	if (m_vertices.empty())
	{
		return;
	}

	m_pBatch->Begin();

	auto size = m_vertices.size();


	for (int i = 0; i < size ; i += 2)
	{
		if (i >= size)
		{
			break;
		}
		DirectX::VertexPositionColor v1(m_vertices[i], DirectX::SimpleMath::Vector4(1.0f, 1.0f, 1.0f, 1.0f));
		DirectX::VertexPositionColor v2(m_vertices[i + 1], DirectX::SimpleMath::Vector4(1.0f, 1.0f, 1.0f, 1.0f));
		m_pBatch->DrawLine(v1, v2);
	}

	m_pBatch->End();
	m_vertices.clear();
	m_colors.clear();
}
