#include "TestLayer.h"

#include "GraphicManager/GraphicManager.h"
#include "Utility/Log.h"
#include "direct3d.h"
#include "Input.h"



void TMF::TestLayer::OnInitialize()
{
	//Log::Info("TestLayer��OnInitialize\n");
	//model = D3D::Get()->LoadObjModel(L"asset/Gun.obj");
	//m_scale = { 0.5f,0.5f,0.5f };
	//m_position = { 0,0,0 };
	//m_rotation = { 0,90,0 };

}

void TMF::TestLayer::OnFinalize()
{
	//Log::Info("TestLayer��OnFinalize\n");
}

void TMF::TestLayer::OnUpdate()
{
	//Log::Info("TestLayer��Update\n");
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
	//	// �������]���Č������x�N�g�������
	//	forward.x = -forward.x;
	//	forward.y = -forward.y;
	//	forward.z = -forward.z;
	//	// �������x�N�g����i�s�����Ƃ��Ĉړ�������
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
	//m_rotation.y += mouseMove.x * 0.1f; // ���E����
	//m_rotation.x += mouseMove.y * 0.1f; // �㉺����

	//if (Input::Get()->GetKeyPress(DIK_UPARROW))
	//{
	//	m_rotation.y += 0.3f;
	//}
	//if (Input::Get()->GetKeyPress(DIK_DOWNARROW))
	//{
	//	m_rotation.y -= 0.3f;
	//}

	//DirectX::XMVECTOR iniForward = DirectX::XMVectorSet(0, 0, 1, 0);

	//// ��]�s��
	//// XYZ�̎O���̉�]�p�x���w�肵�ĉ�]��������@�@���@�I�C���[�p
	//DirectX::XMMATRIX matrixRotateY = DirectX::XMMatrixRotationY(DirectX::XMConvertToRadians(m_rotation.y));
	//DirectX::XMMATRIX matrixRotateX = DirectX::XMMatrixRotationX(DirectX::XMConvertToRadians(m_rotation.x));
	//DirectX::XMMATRIX matrixRotateZ = DirectX::XMMatrixRotationZ(DirectX::XMConvertToRadians(m_rotation.z));

	//// �R���̉�]���܂Ƃ߂��s��
	//DirectX::XMMATRIX matrixRotate = matrixRotateX * matrixRotateY * matrixRotateZ;


	//DirectX::XMVECTOR newForward
	//	= DirectX::XMVector3TransformCoord(iniForward, matrixRotate);

	//// XMVECTOR��XMFLOAT3�ϊ�
	//forward.x = DirectX::XMVectorGetX(newForward);
	//forward.y = DirectX::XMVectorGetY(newForward);
	//forward.z = DirectX::XMVectorGetZ(newForward);

}

void TMF::TestLayer::OnDraw()
{
	//Log::Info("TestLayer��OnDraw\n");

	//DirectX::XMVECTOR iniForward = DirectX::XMVectorSet(1, 0, 0, 0);

	//ID3D11DeviceContext* d3dContext = D3D::Get()->GetContext();

	//D3D::ConstBuffer cb;
	//DirectX::XMMATRIX matrixWorld, matrixView, matrixProj;


	//// Game�N���X�����J��������v�Z�ς݃r���[�ϊ��s����擾
	//matrixView = GraphicManager::Get()->GetViewMatrix();

	//matrixProj = GraphicManager::Get()->GetProjectionMatrix();

	//// ���[���h�ϊ��s��̍쐬
	//// �ړ��s��
	//DirectX::XMMATRIX matrixMove = DirectX::XMMatrixTranslation(
	//	m_position.x, m_position.y, m_position.z);
	//// �g��k���s��
	//DirectX::XMMATRIX matrixScale = DirectX::XMMatrixScaling(m_scale.x, m_scale.y, m_scale.z);
	//// ��]�s��
	//DirectX::XMMATRIX matrixRotate;


	//// XYZ�̎O���̉�]�p�x���w�肵�ĉ�]��������@�@���@�I�C���[�p
	//DirectX::XMMATRIX matrixRotateY = DirectX::XMMatrixRotationY(DirectX::XMConvertToRadians(m_rotation.y));
	//DirectX::XMMATRIX matrixRotateX = DirectX::XMMatrixRotationX(DirectX::XMConvertToRadians(m_rotation.x));
	//DirectX::XMMATRIX matrixRotateZ = DirectX::XMMatrixRotationZ(DirectX::XMConvertToRadians(m_rotation.z));

	//// �R���̉�]���܂Ƃ߂��s��
	//matrixRotate = matrixRotateX * matrixRotateY * matrixRotateZ;

	//DirectX::XMVECTOR newForward
	//	= XMVector3TransformCoord(iniForward, matrixRotate);

	//// XMVECTOR��XMFLOAT3�ϊ�
	//forward.x = DirectX::XMVectorGetX(newForward);
	//forward.y = DirectX::XMVectorGetY(newForward);
	//forward.z = DirectX::XMVectorGetZ(newForward);


	//// �s��͊|���Z�ō����ł��� �i���j�|���鏇�Ԃ��e������
	//matrixWorld =
	//	matrixScale *   // �g��k���s��
	//	matrixRotate *  // ��]�s��
	//	matrixMove;  // �ړ��s��

	//// �@���x�N�g����]�p�s��
	//cb.matrixWorldNormal = XMMatrixTranspose(matrixRotate);

	//cb.matrixWorld = matrixWorld * matrixView * matrixProj;
	//// ���������s��̓]�u�s����쐬���� ���V�F�[�_�[��C++�Ń������̕��т��قȂ邽��
	//cb.matrixWorld = XMMatrixTranspose(cb.matrixWorld);

	//// UV�A�j���[�V�����s��
	//cb.matrixUV = DirectX::XMMatrixIdentity();
	//cb.matrixUV = XMMatrixTranspose(cb.matrixUV);

	//cb.materialDiffuse = { 1,1,1,1 };


	//// �s����V�F�[�_�[�ɓn��
	//d3dContext->UpdateSubresource(D3D::Get()->GetConstantBuffer(), 0, NULL,
	//	&cb, 0, 0);

	//D3D::Model& mdl = model;

	//// ������Draw���钸�_�o�b�t�@�i���f���j���w�肷��
	//UINT strides = D3D::Get()->GetVertexStride();
	//UINT offsets = 0;
	//d3dContext->IASetVertexBuffers(0, 1, &mdl.vertexBuffer,
	//	&strides, &offsets);

	//// �`��Ɏg���C���f�b�N�X�o�b�t�@���w�肷��
	//d3dContext->IASetIndexBuffer(mdl.indexBuffer, DXGI_FORMAT_R16_UINT, 0);

	//// �s�N�Z���V�F�[�_�[�Ƀe�N�X�`����n��
	//d3dContext->PSSetShaderResources(0, 1, &mdl.texture);
	//// ��P�����@���@�`�悷�钸�_��
	//d3dContext->DrawIndexed(mdl.numIndex, 0, 0); // �`�施��
}
