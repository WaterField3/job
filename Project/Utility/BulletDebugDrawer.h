#pragma once
#include "bullet/btBulletDynamicsCommon.h"

#include <d3d11.h>
#include <DirectXMath.h>
#include <vector>
#include <SimpleMath.h>
#include <CommonStates.h>
#include <PrimitiveBatch.h>
#include <VertexTypes.h>

class BulletDebugDrawer : public btIDebugDraw
{
public:

	BulletDebugDrawer(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, ID3D11InputLayout* pInputLayout);
	~BulletDebugDrawer();
	virtual void drawLine(const btVector3& from, const btVector3& to, const btVector3& color) override;
	virtual void drawContactPoint(const btVector3& pointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color) override;
	virtual void reportErrorWarning(const char* warningString) override;
	virtual void draw3dText(const btVector3& location, const char* textString) override;
	virtual void setDebugMode(int debugMode) override;
	virtual int getDebugMode() const override;
	void Render(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, ID3D11InputLayout* pInputLayout);
private:
	int m_debugMode = DBG_DrawWireframe;
	std::vector<DirectX::SimpleMath::Vector3> m_vertices;
	std::vector<DirectX::SimpleMath::Vector4> m_colors;
	std::unique_ptr<DirectX::CommonStates> m_pState;
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>> m_pBatch;
};

