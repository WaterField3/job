#include "TestLayer.h"

#include "GraphicManager/GraphicManager.h"
#include "Utility/Log.h"
#include "direct3d.h"
#include "Input.h"



void TMF::TestLayer::OnInitialize()
{
	//Log::Info("TestLayerのOnInitialize\n");
	//model = D3D::Get()->LoadObjModel(L"asset/Gun.obj");
	//m_scale = { 0.5f,0.5f,0.5f };
	//m_position = { 0,0,0 };
	//m_rotation = { 0,90,0 };

}

void TMF::TestLayer::OnFinalize()
{
	//Log::Info("TestLayerのOnFinalize\n");
}

void TMF::TestLayer::OnUpdate()
{
	//Log::Info("TestLayerのUpdate\n");
	//if (Input::Get()->GetKeyPress(DIK_A))
	//{
	//	m_position.x -= 0.1f;
	//}
	//else if(Input::Get()->GetKeyPress(DIK_D))
	//{
	//	m_position.x += 0.1f;
	//}
	//else if (Input::Get()->GetKeyPress(DIK_S))
	//{
	//	// 符号反転して後ろ向きベクトルを作る
	//	forward.x = -forward.x;
	//	forward.y = -forward.y;
	//	forward.z = -forward.z;
	//	// 後ろ向きベクトルを進行方向として移動させる
	//	float speed = 0.02f;
	//	m_position.x += forward.x * speed;
	//	m_position.y += forward.y * speed;
	//	m_position.z += forward.z * speed;
	//}
	//else if (Input::Get()->GetKeyPress(DIK_W))
	//{
	//	float speed = 0.02f;
	//	m_position.x += forward.x * speed;
	//	m_position.y += forward.y * speed;
	//	m_position.z += forward.z * speed;
	//}

	//POINT mouseMove = Input::Get()->GetMouseMove();
	//m_rotation.y += mouseMove.x * 0.1f; // 左右方向
	//m_rotation.x += mouseMove.y * 0.1f; // 上下方向

	//if (Input::Get()->GetKeyPress(DIK_UPARROW))
	//{
	//	m_rotation.y += 0.3f;
	//}
	//if (Input::Get()->GetKeyPress(DIK_DOWNARROW))
	//{
	//	m_rotation.y -= 0.3f;
	//}

	//DirectX::XMVECTOR iniForward = DirectX::XMVectorSet(0, 0, 1, 0);

	//// 回転行列
	//// XYZの三軸の回転角度を指定して回転させる方法　＝　オイラー角
	//DirectX::XMMATRIX matrixRotateY = DirectX::XMMatrixRotationY(DirectX::XMConvertToRadians(m_rotation.y));
	//DirectX::XMMATRIX matrixRotateX = DirectX::XMMatrixRotationX(DirectX::XMConvertToRadians(m_rotation.x));
	//DirectX::XMMATRIX matrixRotateZ = DirectX::XMMatrixRotationZ(DirectX::XMConvertToRadians(m_rotation.z));

	//// ３軸の回転をまとめた行列
	//DirectX::XMMATRIX matrixRotate = matrixRotateX * matrixRotateY * matrixRotateZ;


	//DirectX::XMVECTOR newForward
	//	= DirectX::XMVector3TransformCoord(iniForward, matrixRotate);

	//// XMVECTOR→XMFLOAT3変換
	//forward.x = DirectX::XMVectorGetX(newForward);
	//forward.y = DirectX::XMVectorGetY(newForward);
	//forward.z = DirectX::XMVectorGetZ(newForward);

}

void TMF::TestLayer::OnDraw()
{
	//Log::Info("TestLayerのOnDraw\n");

	//DirectX::XMVECTOR iniForward = DirectX::XMVectorSet(1, 0, 0, 0);

	//ID3D11DeviceContext* d3dContext = D3D::Get()->GetContext();

	//D3D::ConstBuffer cb;
	//DirectX::XMMATRIX matrixWorld, matrixView, matrixProj;


	//// Gameクラスが持つカメラから計算済みビュー変換行列を取得
	//matrixView = GraphicManager::Get()->GetViewMatrix();

	//matrixProj = GraphicManager::Get()->GetProjectionMatrix();

	//// ワールド変換行列の作成
	//// 移動行列
	//DirectX::XMMATRIX matrixMove = DirectX::XMMatrixTranslation(
	//	m_position.x, m_position.y, m_position.z);
	//// 拡大縮小行列
	//DirectX::XMMATRIX matrixScale = DirectX::XMMatrixScaling(m_scale.x, m_scale.y, m_scale.z);
	//// 回転行列
	//DirectX::XMMATRIX matrixRotate;


	//// XYZの三軸の回転角度を指定して回転させる方法　＝　オイラー角
	//DirectX::XMMATRIX matrixRotateY = DirectX::XMMatrixRotationY(DirectX::XMConvertToRadians(m_rotation.y));
	//DirectX::XMMATRIX matrixRotateX = DirectX::XMMatrixRotationX(DirectX::XMConvertToRadians(m_rotation.x));
	//DirectX::XMMATRIX matrixRotateZ = DirectX::XMMatrixRotationZ(DirectX::XMConvertToRadians(m_rotation.z));

	//// ３軸の回転をまとめた行列
	//matrixRotate = matrixRotateX * matrixRotateY * matrixRotateZ;

	//DirectX::XMVECTOR newForward
	//	= XMVector3TransformCoord(iniForward, matrixRotate);

	//// XMVECTOR→XMFLOAT3変換
	//forward.x = DirectX::XMVectorGetX(newForward);
	//forward.y = DirectX::XMVectorGetY(newForward);
	//forward.z = DirectX::XMVectorGetZ(newForward);


	//// 行列は掛け算で合成できる （注）掛ける順番が影響する
	//matrixWorld =
	//	matrixScale *   // 拡大縮小行列
	//	matrixRotate *  // 回転行列
	//	matrixMove;  // 移動行列

	//// 法線ベクトル回転用行列
	//cb.matrixWorldNormal = XMMatrixTranspose(matrixRotate);

	//cb.matrixWorld = matrixWorld * matrixView * matrixProj;
	//// 合成した行列の転置行列を作成する ※シェーダーとC++でメモリの並びが異なるため
	//cb.matrixWorld = XMMatrixTranspose(cb.matrixWorld);

	//// UVアニメーション行列
	//cb.matrixUV = DirectX::XMMatrixIdentity();
	//cb.matrixUV = XMMatrixTranspose(cb.matrixUV);

	//cb.materialDiffuse = { 1,1,1,1 };


	//// 行列をシェーダーに渡す
	//d3dContext->UpdateSubresource(D3D::Get()->GetConstantBuffer(), 0, NULL,
	//	&cb, 0, 0);

	//D3D::Model& mdl = model;

	//// 今からDrawする頂点バッファ（モデル）を指定する
	//UINT strides = D3D::Get()->GetVertexStride();
	//UINT offsets = 0;
	//d3dContext->IASetVertexBuffers(0, 1, &mdl.vertexBuffer,
	//	&strides, &offsets);

	//// 描画に使うインデックスバッファを指定する
	//d3dContext->IASetIndexBuffer(mdl.indexBuffer, DXGI_FORMAT_R16_UINT, 0);

	//// ピクセルシェーダーにテクスチャを渡す
	//d3dContext->PSSetShaderResources(0, 1, &mdl.texture);
	//// 第１引数　→　描画する頂点数
	//d3dContext->DrawIndexed(mdl.numIndex, 0, 0); // 描画命令
}
