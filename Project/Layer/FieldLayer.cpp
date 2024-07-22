#include "FieldLayer.h"

#include "GraphicManager/GraphicManager.h"

void TMF::FieldLayer::OnInitialize()
{
	model = D3D::Get()->LoadObjModel(L"asset/Road00.obj");
	m_scale = { 0.5f,0.5f,0.5f };
	m_position = { 0,-1,0 };
	m_rotation = { 0,90,0 };
}

void TMF::FieldLayer::OnFinalize()
{
}

void TMF::FieldLayer::OnUpdate()
{
}

void TMF::FieldLayer::OnDraw()
{
	ID3D11DeviceContext* d3dContext = D3D::Get()->GetContext();

	D3D::ConstBuffer cb;
	DirectX::XMMATRIX matrixWorld, matrixView, matrixProj;


	// Gameクラスが持つカメラから計算済みビュー変換行列を取得
	matrixView = GraphicManager::Get()->GetViewMatrix();

	matrixProj = GraphicManager::Get()->GetProjectionMatrix();

	DirectX::XMMATRIX matrixMove = DirectX::XMMatrixTranslation(
		m_position.x, m_position.y, m_position.z);
	// 拡大縮小行列
	DirectX::XMMATRIX matrixScale = DirectX::XMMatrixScaling(m_scale.x, m_scale.y, m_scale.z);
	// 回転行列
	DirectX::XMMATRIX matrixRotate;


	// XYZの三軸の回転角度を指定して回転させる方法　＝　オイラー角
	DirectX::XMMATRIX matrixRotateY = DirectX::XMMatrixRotationY(DirectX::XMConvertToRadians(m_rotation.y));
	DirectX::XMMATRIX matrixRotateX = DirectX::XMMatrixRotationX(DirectX::XMConvertToRadians(m_rotation.x));
	DirectX::XMMATRIX matrixRotateZ = DirectX::XMMatrixRotationZ(DirectX::XMConvertToRadians(m_rotation.z));

	// ３軸の回転をまとめた行列
	matrixRotate = matrixRotateX * matrixRotateY * matrixRotateZ;

	// 行列は掛け算で合成できる （注）掛ける順番が影響する
	matrixWorld =
		matrixScale *   // 拡大縮小行列
		matrixRotate *  // 回転行列
		matrixMove;  // 移動行列

	// 法線ベクトル回転用行列
	cb.matrixWorldNormal = XMMatrixTranspose(matrixRotate);

	cb.matrixWorld = matrixWorld * matrixView * matrixProj;
	// 合成した行列の転置行列を作成する ※シェーダーとC++でメモリの並びが異なるため
	cb.matrixWorld = XMMatrixTranspose(cb.matrixWorld);

	// UVアニメーション行列
	cb.matrixUV = DirectX::XMMatrixIdentity();
	cb.matrixUV = XMMatrixTranspose(cb.matrixUV);

	cb.materialDiffuse = { 1,1,1,1 };


	// 行列をシェーダーに渡す
	d3dContext->UpdateSubresource(D3D::Get()->GetConstantBuffer(), 0, NULL,
		&cb, 0, 0);

	D3D::Model& mdl = model;

	// 今からDrawする頂点バッファ（モデル）を指定する
	UINT strides = D3D::Get()->GetVertexStride();
	UINT offsets = 0;
	d3dContext->IASetVertexBuffers(0, 1, &mdl.vertexBuffer,
		&strides, &offsets);

	// 描画に使うインデックスバッファを指定する
	d3dContext->IASetIndexBuffer(mdl.indexBuffer, DXGI_FORMAT_R16_UINT, 0);

	// ピクセルシェーダーにテクスチャを渡す
	d3dContext->PSSetShaderResources(0, 1, &mdl.texture);
	// 第１引数　→　描画する頂点数
	d3dContext->DrawIndexed(mdl.numIndex, 0, 0); // 描画命令
}

void TMF::FieldLayer::SetName(const char name[64])
{
}

const DirectX::XMFLOAT3 TMF::FieldLayer::GetTransform(const char name[32])
{
	return DirectX::XMFLOAT3();
}

const char* TMF::FieldLayer::GetName()
{
	return nullptr;
}
