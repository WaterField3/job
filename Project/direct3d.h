#pragma once // �d���C���N���[�h�������h�~����

#include <d3d11.h>  // DirectX11���g�����߂̃w�b�_�[�t�@�C��
#include <DirectXMath.h> // DirextX�̐��w�֘A�̃w�b�_�[�t�@�C��
#include <SimpleMath.h>
#include <Effects.h>
#include <CommonStates.h>
#include <memory>
#include <wrl/client.h>
#include <PostProcess.h>
#include <Effects.h>
#include <string>

#include "RenderTexture.h"
#include "SpriteBatch.h"
#include "VertexTypes.h"

// Direct3D����̊ȗ����}�N��
#define SAFE_RELEASE(p)      { if( NULL != p ) { p->Release(); p = NULL; } }

// �����̌^����ʖ��ł��g����悤�ɂ���
typedef ID3D11ShaderResourceView* D3DTEXTURE;
typedef DirectX::XMFLOAT2 FLOAT_XY;
constexpr int MAX_BONE = 400;

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

	struct WEIGHT {
		std::string bonename;						// �{�[����
		std::string meshname;						// ���b�V����
		float weight;								// �E�F�C�g�l
		int	vertexindex;							// ���_�C���f�b�N�X
	};

	//�{�[���\���́@20231231 �C���iDX���j
	struct BONE
	{
		std::string bonename;							// �{�[����
		std::string meshname;							// ���b�V����
		std::string armaturename;						// �A�[�}�`���A��
		//	aiMatrix4x4 Matrix{};						// �e�q�֌W���l�������s��
		//	aiMatrix4x4 AnimationMatrix{};				// �����̎��������l�����s��
		//	aiMatrix4x4 OffsetMatrix{};					// �{�[���I�t�Z�b�g�s��
		DirectX::SimpleMath::Matrix Matrix{};			// �e�q�֌W���l�������s��
		DirectX::SimpleMath::Matrix AnimationMatrix{};	// �����̎��������l�����s��
		DirectX::SimpleMath::Matrix OffsetMatrix{};		// �{�[���I�t�Z�b�g�s��
		int			idx;								// �z��̉��Ԗڂ�			
		std::vector<WEIGHT> weights;					// ���̃{�[�����e����^���钸�_�C���f�b�N�X�E�E�F�C�g�l

		DirectX::SimpleMath::Quaternion	fromQuat{};		// from�N�H�[�^�j�I��
		DirectX::SimpleMath::Quaternion	toQuat{};		// to�N�H�[�^�j�I��

		DirectX::SimpleMath::Vector3	fromPos{};		// from�ʒu
		DirectX::SimpleMath::Vector3	toPos{};		// to�ʒu

		DirectX::SimpleMath::Vector3	fromScale{};	// from�X�P�[��
		DirectX::SimpleMath::Vector3	toScale{};		// to�X�P�[��
	};

	// �R�c���_�f�[�^
	struct VERTEX_3D
	{
		DirectX::SimpleMath::Vector3	Position;
		DirectX::SimpleMath::Vector3	Normal;
		DirectX::SimpleMath::Color		Diffuse;
		DirectX::SimpleMath::Vector2	TexCoord;
		int		BoneIndex[4];			// 20231225
		float	BoneWeight[4];			// 20231225
		//	std::string	BoneName[4];		// 20231226
		int		bonecnt = 0;			// 20231226
	};


	// �}�e���A��
	struct MATERIAL
	{
		DirectX::SimpleMath::Color	Ambient;
		DirectX::SimpleMath::Color	Diffuse;
		DirectX::SimpleMath::Color	Specular;
		DirectX::SimpleMath::Color	Emission;
		float		Shiness;
		BOOL		TextureEnable;
		float		Dummy[2]{};
	};


	// ���s����
	struct LIGHT
	{
		BOOL		Enable;
		BOOL		Dummy[3];
		DirectX::SimpleMath::Vector4	Direction;
		DirectX::SimpleMath::Color	Diffuse;
		DirectX::SimpleMath::Color	Ambient;
	};

	// ���b�V���i�}�e���A�����ɃT�u�Z�b�g�����݂���j
	struct SUBSET {
		std::string		MtrlName;						// �}�e���A����
		unsigned int	IndexNum = 0;					// �C���f�b�N�X��
		unsigned int	VertexNum = 0;					// ���_��
		unsigned int	IndexBase = 0;					// �J�n�C���f�b�N�X
		unsigned int	VertexBase = 0;					// ���_�x�[�X
		unsigned int	MaterialIdx = 0;				// �}�e���A���C���f�b�N�X
	};

	// �u�����h�X�e�[�g
	enum EBlendState {
		BS_NONE = 0,							// ��������������
		BS_ALPHABLEND,							// ����������
		BS_ADDITIVE,							// ���Z����
		BS_SUBTRACTION,							// ���Z����
		MAX_BLENDSTATE
	};

	// �{�[���R���r�l�[�V�����s��

	struct CBBoneCombMatrix {
		DirectX::XMFLOAT4X4 BoneCombMtx[MAX_BONE];	// �{�[���R���r�l�[�V�����s��		// 20240713
	};

	enum BloomPresets
	{
		Default = 0,
		Soft,
		Desaturated,
		Saturated,
		Blurry,
		Subtle,
		None
	};

	// Direct3D�̏��������������s����
	HRESULT Create(HWND hwnd);
	void Init();
	D3D() {};
	~D3D();

	// ��ʓh��Ԃ��Ɛݒ���s���֐�
	void ClearScreen();
	// ��ʂ��X�V����֐�
	void UpdateScreen();
	void SettingEffect(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj);

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
	ID3D11InputLayout* GetInputLayout() { return m_pInputLayout; }
	inline HWND GetHwnd() const { return m_hwnd; }
	inline std::shared_ptr<DirectX::CommonStates> GetCommonStates() { return m_pCommonStates; }

	// ���_�f�[�^�P������̃o�C�g�T�C�Y��Ԃ�
	UINT GetVertexStride();
	void PostProcess();
	inline BloomPresets GetBloomPresets() { return g_Bloom; }
	void SetBloomPresets(BloomPresets set);
	inline std::weak_ptr<DirectX::SpriteBatch> GetSpriteBatch() const { return m_pSpriteBatch; }
	inline RECT GetFullScreenRect() { return m_fullscreenRect; }
private:
	//D3D() {};


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

	ID3D11Resource* m_pResource;

	HWND m_hwnd;

	std::shared_ptr<DirectX::BasicEffect> m_pEffect;

	std::shared_ptr<DirectX::SpriteBatch> m_pSpriteBatch;

	std::shared_ptr<DirectX::CommonStates> m_pCommonStates;

	//Microsoft::WRL::ComPtr <ID3D11ShaderResourceView> m_pBackGround;

	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_bloomExtractPS;
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_bloomCombinePS;
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_gaussianBlurPS;

	Microsoft::WRL::ComPtr<ID3D11Buffer> m_bloomParams;
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_blurParamsWidth;
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_blurParamsHeight;

	std::unique_ptr<DX::RenderTexture> m_offscreenTexture;
	std::unique_ptr<DX::RenderTexture> m_renderTarget1;
	std::unique_ptr<DX::RenderTexture> m_renderTarget2;

	RECT m_bloomRect;
	RECT m_fullscreenRect;
	RECT m_size;

	BloomPresets g_Bloom = None;
};

