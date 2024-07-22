#include<SimpleMath.h>

#include "BoxMesh.h"
#include "GraphicManager/GraphicManager.h"



#pragma comment(lib,"directxtk.lib")

void BoxMesh::Initialize(DirectX::XMFLOAT3 _scale, DirectX::XMFLOAT4 _color)
{
    // サイズの設定
    m_length = _scale;

    // 色の設定
    m_color = _color;

    // インデックスデータの作成
    CreateIndex();

    // 頂点データの作成
    CreateVertex();

}

void BoxMesh::Draw()
{
	ID3D11DeviceContext* deviceContext = D3D::Get()->GetContext();

	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);



}

void BoxMesh::Normalize(DirectX::XMFLOAT3 _vector, DirectX::XMFLOAT3& _normal)
{
    DirectX::XMFLOAT3 rv{ _vector };

    auto&& vec{ DirectX::XMLoadFloat3(&rv) };

    DirectX::XMStoreFloat3(&rv, DirectX::XMVector3Normalize(vec));

    _normal = rv;
}
void BoxMesh::CreateIndex()
{
   
}

void BoxMesh::CreateVertex()
{
	// 頂点クリア
	m_vertices.clear();

	m_vertices.resize(12);
	m_vertices[0].position.x = -(m_length.x / 2.0f);
	m_vertices[0].position.y = (m_length.y / 2.0f);
	m_vertices[0].position.z = -(m_length.z / 2.0f);

	m_vertices[1].position.x = (m_length.x / 2.0f);
	m_vertices[1].position.y = (m_length.y / 2.0f);
	m_vertices[1].position.z = -(m_length.z / 2.0f);

	m_vertices[2].position.x = (m_length.x / 2.0f);
	m_vertices[2].position.y = -(m_length.y / 2.0f);
	m_vertices[2].position.z = -(m_length.z / 2.0f);

	m_vertices[3].position.x = -(m_length.x / 2.0f);
	m_vertices[3].position.y = -(m_length.y / 2.0f);
	m_vertices[3].position.z = -(m_length.z / 2.0f);


	m_vertices[4].position.x = -(m_length.x / 2.0f);
	m_vertices[4].position.y = (m_length.y / 2.0f);
	m_vertices[4].position.z = (m_length.z / 2.0f);

	m_vertices[5].position.x = (m_length.x / 2.0f);
	m_vertices[5].position.y = (m_length.y / 2.0f);
	m_vertices[5].position.z = (m_length.z / 2.0f);

	m_vertices[6].position.x = (m_length.x / 2.0f);
	m_vertices[6].position.y = -(m_length.y / 2.0f);
	m_vertices[6].position.z = (m_length.z / 2.0f);

	m_vertices[7].position.x = -(m_length.x / 2.0f);
	m_vertices[7].position.y = -(m_length.y / 2.0f);
	m_vertices[7].position.z = (m_length.z / 2.0f);


	for (int i = 0; i < 8; i++) {
		// 頂点カラー設定
		m_vertices[i].diffuse = m_color;
		// ノーマライズ
		Normalize(
			m_vertices[i].position,
			m_vertices[i].normal);
	}
}

