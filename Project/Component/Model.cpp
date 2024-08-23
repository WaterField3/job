#include "Model.h"

#include <Imgui/imgui.h>

#include "GameObject/GameObjectManager.h"
#include "Transform.h"
#include "Camera.h"
#include "direct3d.h"

REGISTER_COMPONENT(TMF::Model, "Model");

namespace TMF
{
	Model::Model()
	{

	}

	Model::~Model()
	{

	}

	void Model::OnInitialize()
	{
		//auto wideFileName = std::wstring(m_loadFileName.begin(), m_loadFileName.end());
		//m_model = D3D::Get()->LoadObjModel(wideFileName.c_str());
		auto wideFileName = std::wstring(m_loadCmo.begin(), m_loadCmo.end());
		auto device = D3D::Get()->GetDevice();
		m_pCommonState = std::make_unique<DirectX::CommonStates>(device);
		m_pEffectFactory = std::make_unique<DirectX::EffectFactory>(device);
		m_pModel = DirectX::Model::CreateFromCMO(device, wideFileName.c_str(), *m_pEffectFactory);
	}

	void Model::OnFinalize()
	{

	}

	void Model::OnUpdate()
	{

	}

	void Model::OnLateUpdate()
	{

	}

	void Model::OnDraw()
	{

		if (m_isDraw)
		{
			ModelDraw();
		}
	}

	void Model::OnDrawImGui()
	{
		ImGui::Checkbox("active", &m_isDraw);
		char buf[256] = "";
		strcpy_s(buf, sizeof(buf), m_loadFileName.c_str());
		if (ImGui::InputText("fileName", buf, 256))
		{
			m_loadFileName = buf;
		}
		if (ImGui::Button("LoadModel"))
		{
			//auto wideFileName = std::wstring(m_loadFileName.begin(), m_loadFileName.end());
			//m_model = D3D::Get()->LoadObjModel(wideFileName.c_str());
		}
		if (ImGui::Button("LoadCmo"))
		{
			auto wideFileName = std::wstring(m_loadFileName.begin(), m_loadFileName.end());
			m_pModel = DirectX::Model::CreateFromCMO(D3D::Get()->GetDevice(), wideFileName.c_str(), *m_pEffectFactory);
		}
	}

	boost::uuids::uuid Model::OnGetUUID()
	{
		return m_uuID;
	}

	void Model::ModelDraw()
	{
		ID3D11DeviceContext* d3dContext = D3D::Get()->GetContext();

		D3D::ConstBuffer cb;
		DirectX::SimpleMath::Matrix matrixWorld;

		auto view = DirectX::SimpleMath::Matrix::CreateLookAt(DirectX::SimpleMath::Vector3(0.0f, 0.0f, 5.0f), DirectX::SimpleMath::Vector3::Zero, DirectX::SimpleMath::Vector3::UnitY);
		auto proj = DirectX::SimpleMath::Matrix::CreatePerspectiveFieldOfView(DirectX::XM_PI / 4.0f, float(1024) / float(576), 0.1f, 10.f);
		for (auto pGameObject : GameObjectManager::Instance().GetGameObjects())
		{
			if (pGameObject->GetName() == "CameraObject")
			{
				auto cameraComponent = pGameObject->GetComponent<Camera>();
				if (auto camera = cameraComponent.lock())
				{
					view = camera->GetViewMatrix();
					proj = camera->GetProjectionMatrix();
				}
			}
		}

		// XYZの三軸の回転角度を指定して回転させる方法　＝　オイラー角
		auto matrixRotateX = DirectX::SimpleMath::Matrix::CreateRotationX(0);
		auto matrixRotateY = DirectX::SimpleMath::Matrix::CreateRotationY(0);
		auto matrixRotateZ = DirectX::SimpleMath::Matrix::CreateRotationZ(0);

		// 回転行列
		auto matrixRotate = matrixRotateX * matrixRotateY * matrixRotateZ;

		if (auto gameObject = m_pOwner.lock())
		{
			auto Component = gameObject->GetComponent<Transform>();
			if (auto pComponent = Component.lock())
			{
				matrixWorld = pComponent->GetMatrixLocal();
				matrixRotate = pComponent->GetMatrixRotation();
			}
		}

		if (m_pModel)
		{
			m_pModel->Draw(d3dContext, *m_pCommonState, matrixWorld, view, proj);
		}

		//// 法線ベクトル回転用行列
		////cb.matrixWorldNormal = matrixRotate.Transpose();

		//cb.matrixWorld = matrixWorld * view * proj;
		//// 合成した行列の転置行列を作成する ※シェーダーとC++でメモリの並びが異なるため
		//cb.matrixWorld = cb.matrixWorld.Transpose();

		//// UVアニメーション行列
		//cb.matrixUV = DirectX::XMMatrixIdentity();
		//cb.matrixUV = cb.matrixUV.Transpose();

		//cb.materialDiffuse = { 1,1,1,1 };


		//// 行列をシェーダーに渡す
		//d3dContext->UpdateSubresource(D3D::Get()->GetConstantBuffer(), 0, NULL,
		//	&cb, 0, 0);

		//D3D::Model& mdl = m_model;

		//// 今からDrawする頂点バッファ（モデル）を指定する
		//UINT strides = D3D::Get()->GetVertexStride();
		//UINT offsets = 0;
		//d3dContext->IASetVertexBuffers(0, 1, &mdl.vertexBuffer,
		//	&strides, &offsets);

		//// 描画に使うインデックスバッファを指定する
		//d3dContext->IASetIndexBuffer(mdl.indexBuffer, DXGI_FORMAT_R16_UINT, 0);

		//// ピクセルシェーダーにテクスチャを渡す
		//d3dContext->PSSetShaderResources(0, 1, &mdl.texture);


		//d3dContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		//// 第１引数　→　描画する頂点数
		////d3dContext->DrawIndexed(mdl.numIndex, 0, 0); // 描画命令
	}

}
