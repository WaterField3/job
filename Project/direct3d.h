#pragma once // �d���C���N���[�h�������h�~����

#include <d3d11.h>  // DirectX11���g�����߂̃w�b�_�[�t�@�C��
#include <DirectXMath.h> // DirextX�̐��w�֘A�̃w�b�_�[�t�@�C��
#include <SimpleMath.h>

// Direct3D����̊ȗ����}�N��
#define SAFE_RELEASE(p)      { if( NULL != p ) { p->Release(); p = NULL; } }

// �����̌^����ʖ��ł��g����悤�ɂ���
typedef ID3D11ShaderResourceView* D3DTEXTURE;
typedef DirectX::XMFLOAT2 FLOAT_XY;

// Direct3D�̋@�\���J�v�Z���������N���X
class D3D
{

public:
	// �\���̂̒�`
	// �F��\��
	struct Color
	{
		float r, g, b, a;
	};
	// ���f����\��
	struct Model
	{
		ID3D11Buffer* vertexBuffer; // ���_�o�b�t�@�����_�f�[�^������
		D3DTEXTURE texture; // �e�N�X�`��
		ID3D11Buffer* indexBuffer; // �C���f�b�N�X�o�b�t�@
		int numIndex; // �C���f�b�N�X��
	};
	// �萔�o�b�t�@�p�\����
	struct ConstBuffer
	{
		// ���[���h�ϊ��s��
		DirectX::SimpleMath::Matrix matrixWorld;

		// �@���x�N�g������]������s��
		DirectX::SimpleMath::Matrix matrixWorldNormal;

		// UV�A�j���[�V�����s��
		DirectX::SimpleMath::Matrix matrixUV;

		// �}�e���A���F
		DirectX::SimpleMath::Vector4 materialDiffuse;

		unsigned int isLighting;
		int dummy2, dummy3, dummy4;
	};

	// �\���̂̒�`
// ���_�f�[�^��\���\����
	struct Vertex
	{
		// ���_�̈ʒu���W
		DirectX::SimpleMath::Vector3 position;
		// UV���W�i�e�N�X�`�����W�j
		DirectX::SimpleMath::Vector2 uv;
		// ���_�̖@���x�N�g��
		DirectX::SimpleMath::Vector3 normal;
		// ���_�̊g�U
		DirectX::SimpleMath::Vector4 diffuse;

	};

	// Direct3D�̏��������������s����
	HRESULT Create(HWND hwnd);
	~D3D();

	// ��ʓh��Ԃ��Ɛݒ���s���֐�
	void ClearScreen();
	// ��ʂ��X�V����֐�
	void UpdateScreen();

	// �l�p�`�|���S�����쐬���A���_�o�b�t�@�������s��
	static Model CreateSquare(FLOAT_XY center, FLOAT_XY size, FLOAT_XY uv);
	// Obj�t�@�C����ǂݍ���
	// �����F(1)�t�@�C����
	static Model LoadObjModel(const wchar_t* fileName);
	// �e�N�X�`����ǂݍ���
	// �����F(1)�t�@�C����
	static D3DTEXTURE LoadTexture(const wchar_t* fileName);
	// ���̃N���X�̗B��̃C���X�^���X��Ԃ�
	static D3D* Get();

	static DirectX::XMFLOAT3 RotateVector3(DirectX::XMFLOAT3 srcVector, DirectX::XMFLOAT3 rotation);

	ID3D11Device* GetDevice() { return m_pDevice; }
	ID3D11DeviceContext* GetContext() { return m_pImmediateContext; }
	ID3D11Buffer* GetConstantBuffer() { return m_pConstantBuffer; }

	inline HWND GetHwnd() const { return m_hwnd; }

	// ���_�f�[�^�P������̃o�C�g�T�C�Y��Ԃ�
	UINT GetVertexStride();

private:
	D3D() {};

	// ��ID3D11�Ŏn�܂�|�C���^�^�̕ϐ��́A�������K�v������
	ID3D11Device* m_pDevice; // �f�o�C�X��DirectX�̊e��@�\�����
	// �R���e�L�X�g���`��֘A���i��@�\
	ID3D11DeviceContext* m_pImmediateContext;
	// �X���b�v�`�F�C�����_�u���o�b�t�@�@�\
	IDXGISwapChain* m_pSwapChain;
	// �����_�[�^�[�Q�b�g���`����\���@�\
	ID3D11RenderTargetView* m_pRenderTargetView;
	// �[�x�o�b�t�@�p�e�N�X�`��
	ID3D11Texture2D* m_pDepthStencilTexture;
	// �[�x�o�b�t�@
	ID3D11DepthStencilView* m_pDepthStencilView;

	// �C���v�b�g���C�A�E�g
	ID3D11InputLayout* m_pInputLayout;
	// ���_�V�F�[�_�[�I�u�W�F�N�g
	ID3D11VertexShader* m_pVertexShader;
	// �s�N�Z���V�F�[�_�[�I�u�W�F�N�g
	ID3D11PixelShader* m_pPixelShader;
	// �r���[�|�[�g
	D3D11_VIEWPORT          m_Viewport;

	// �u�����h�X�e�[�g�p�ϐ��i�A���t�@�u�����f�B���O�j
	ID3D11BlendState* m_pBlendStateAlpha;
	// �u�����h�X�e�[�g�p�ϐ��i���Z�����j
	ID3D11BlendState* m_pBlendStateAdditive;

	// �T���v���[�p�ϐ�
	ID3D11SamplerState* m_pSampler;

	// �萔�o�b�t�@�p�ϐ�
	ID3D11Buffer* m_pConstantBuffer;

	HWND m_hwnd;
};

