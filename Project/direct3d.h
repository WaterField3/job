#pragma once // 重複インクルードを自動防止する

#include <d3d11.h>  // DirectX11を使うためのヘッダーファイル
#include <DirectXMath.h> // DirextXの数学関連のヘッダーファイル
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

// Direct3D解放の簡略化マクロ
#define SAFE_RELEASE(p)      { if( NULL != p ) { p->Release(); p = NULL; } }

// 既存の型名を別名でも使えるようにする
typedef ID3D11ShaderResourceView* D3DTEXTURE;
typedef DirectX::XMFLOAT2 FLOAT_XY;
constexpr int MAX_BONE = 400;

// Direct3Dの機能をカプセル化したクラス
class D3D
{

public:
	// 構造体の定義
	// 色を表す
	struct Color
	{
		float r, g, b, a;
	};
	// モデルを表す
	struct Model
	{
		ID3D11Buffer* vertexBuffer; // 頂点バッファ＝頂点データを持つ
		D3DTEXTURE texture; // テクスチャ
		ID3D11Buffer* indexBuffer; // インデックスバッファ
		int numIndex; // インデックス数
	};
	// 定数バッファ用構造体
	struct ConstBuffer
	{
		// ワールド変換行列
		DirectX::SimpleMath::Matrix matrixWorld;

		// 法線ベクトルを回転させる行列
		DirectX::SimpleMath::Matrix matrixWorldNormal;

		// UVアニメーション行列
		DirectX::SimpleMath::Matrix matrixUV;

		// マテリアル色
		DirectX::SimpleMath::Vector4 materialDiffuse;

		unsigned int isLighting;
		int dummy2, dummy3, dummy4;
	};

	// 構造体の定義
// 頂点データを表す構造体
	struct Vertex
	{
		// 頂点の位置座標
		DirectX::SimpleMath::Vector3 position;
		// UV座標（テクスチャ座標）
		DirectX::SimpleMath::Vector2 uv;
		// 頂点の法線ベクトル
		DirectX::SimpleMath::Vector3 normal;
		// 頂点の拡散
		DirectX::SimpleMath::Vector4 diffuse;

	};

	struct WEIGHT {
		std::string bonename;						// ボーン名
		std::string meshname;						// メッシュ名
		float weight;								// ウェイト値
		int	vertexindex;							// 頂点インデックス
	};

	//ボーン構造体　20231231 修正（DX化）
	struct BONE
	{
		std::string bonename;							// ボーン名
		std::string meshname;							// メッシュ名
		std::string armaturename;						// アーマチュア名
		//	aiMatrix4x4 Matrix{};						// 親子関係を考慮した行列
		//	aiMatrix4x4 AnimationMatrix{};				// 自分の事だけを考えた行列
		//	aiMatrix4x4 OffsetMatrix{};					// ボーンオフセット行列
		DirectX::SimpleMath::Matrix Matrix{};			// 親子関係を考慮した行列
		DirectX::SimpleMath::Matrix AnimationMatrix{};	// 自分の事だけを考えた行列
		DirectX::SimpleMath::Matrix OffsetMatrix{};		// ボーンオフセット行列
		int			idx;								// 配列の何番目か			
		std::vector<WEIGHT> weights;					// このボーンが影響を与える頂点インデックス・ウェイト値

		DirectX::SimpleMath::Quaternion	fromQuat{};		// fromクォータニオン
		DirectX::SimpleMath::Quaternion	toQuat{};		// toクォータニオン

		DirectX::SimpleMath::Vector3	fromPos{};		// from位置
		DirectX::SimpleMath::Vector3	toPos{};		// to位置

		DirectX::SimpleMath::Vector3	fromScale{};	// fromスケール
		DirectX::SimpleMath::Vector3	toScale{};		// toスケール
	};

	// ３Ｄ頂点データ
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


	// マテリアル
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


	// 平行光源
	struct LIGHT
	{
		BOOL		Enable;
		BOOL		Dummy[3];
		DirectX::SimpleMath::Vector4	Direction;
		DirectX::SimpleMath::Color	Diffuse;
		DirectX::SimpleMath::Color	Ambient;
	};

	// メッシュ（マテリアル毎にサブセットが存在する）
	struct SUBSET {
		std::string		MtrlName;						// マテリアル名
		unsigned int	IndexNum = 0;					// インデックス数
		unsigned int	VertexNum = 0;					// 頂点数
		unsigned int	IndexBase = 0;					// 開始インデックス
		unsigned int	VertexBase = 0;					// 頂点ベース
		unsigned int	MaterialIdx = 0;				// マテリアルインデックス
	};

	// ブレンドステート
	enum EBlendState {
		BS_NONE = 0,							// 半透明合成無し
		BS_ALPHABLEND,							// 半透明合成
		BS_ADDITIVE,							// 加算合成
		BS_SUBTRACTION,							// 減算合成
		MAX_BLENDSTATE
	};

	// ボーンコンビネーション行列

	struct CBBoneCombMatrix {
		DirectX::XMFLOAT4X4 BoneCombMtx[MAX_BONE];	// ボーンコンビネーション行列		// 20240713
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

	// Direct3Dの初期化処理を実行する
	HRESULT Create(HWND hwnd);
	void Init();
	D3D() {};
	~D3D();

	// 画面塗りつぶしと設定を行う関数
	void ClearScreen();
	// 画面を更新する関数
	void UpdateScreen();
	void SettingEffect(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj);

	// 四角形ポリゴンを作成し、頂点バッファ生成を行う
	static Model CreateSquare(FLOAT_XY center, FLOAT_XY size, FLOAT_XY uv);
	// Objファイルを読み込む
	// 引数：(1)ファイル名
	static Model LoadObjModel(const wchar_t* fileName);
	// テクスチャを読み込む
	// 引数：(1)ファイル名
	static D3DTEXTURE LoadTexture(const wchar_t* fileName);
	// このクラスの唯一のインスタンスを返す
	static D3D* Get();

	static DirectX::XMFLOAT3 RotateVector3(DirectX::XMFLOAT3 srcVector, DirectX::XMFLOAT3 rotation);

	ID3D11Device* GetDevice() { return m_pDevice; }
	ID3D11DeviceContext* GetContext() { return m_pImmediateContext; }
	ID3D11Buffer* GetConstantBuffer() { return m_pConstantBuffer; }
	ID3D11InputLayout* GetInputLayout() { return m_pInputLayout; }
	inline HWND GetHwnd() const { return m_hwnd; }
	inline std::shared_ptr<DirectX::CommonStates> GetCommonStates() { return m_pCommonStates; }
	inline void GetSwapChainFullScreenState(BOOL &fullscreenState) { m_pSwapChain->GetFullscreenState(&fullscreenState, nullptr); }
	void ChangeSwapChainFullScreenState(BOOL fullscreenState) const { m_pSwapChain->SetFullscreenState(!fullscreenState, nullptr); }

	// 頂点データ１つあたりのバイトサイズを返す
	UINT GetVertexStride();
	void PostProcess();
	inline BloomPresets GetBloomPresets() { return g_Bloom; }
	void SetBloomPresets(BloomPresets set);
	inline std::weak_ptr<DirectX::SpriteBatch> GetSpriteBatch() const { return m_pSpriteBatch; }
	inline RECT GetFullScreenRect() { return m_fullscreenRect; }

	//void SetWorldViewProjection2D();
	void SetWorldMatrix(DirectX::SimpleMath::Matrix* WorldMatrix);
	void SetViewMatrix(DirectX::SimpleMath::Matrix* ViewMatrix);
	void SetProjectionMatrix(DirectX::SimpleMath::Matrix* ProjectionMatrix);
	void SetMaterial(MATERIAL Material);
	void SetLight(LIGHT Light);

	//void CreateSkinnedEffect();
private:
	//D3D() {};


	// ※ID3D11で始まるポインタ型の変数は、解放する必要がある
	ID3D11Device* m_pDevice; // デバイス＝DirectXの各種機能を作る
	// コンテキスト＝描画関連を司る機能
	ID3D11DeviceContext* m_pImmediateContext;
	// スワップチェイン＝ダブルバッファ機能
	IDXGISwapChain* m_pSwapChain;
	// レンダーターゲット＝描画先を表す機能
	ID3D11RenderTargetView* m_pRenderTargetView;
	// 深度バッファ用テクスチャ
	ID3D11Texture2D* m_pDepthStencilTexture;
	// 深度バッファ
	ID3D11DepthStencilView* m_pDepthStencilView;

	// インプットレイアウト
	ID3D11InputLayout* m_pInputLayout;
	// 頂点シェーダーオブジェクト
	ID3D11VertexShader* m_pVertexShader;
	// ピクセルシェーダーオブジェクト
	ID3D11PixelShader* m_pPixelShader;
	// ビューポート
	D3D11_VIEWPORT          m_Viewport;

	// ブレンドステート用変数（アルファブレンディング）
	ID3D11BlendState* m_pBlendStateAlpha;
	// ブレンドステート用変数（加算合成）
	ID3D11BlendState* m_pBlendStateAdditive;

	// サンプラー用変数
	ID3D11SamplerState* m_pSampler;

	// 定数バッファ用変数
	ID3D11Buffer* m_pConstantBuffer;
	ID3D11Buffer* m_pWorldBuffer;
	ID3D11Buffer* m_pProjectionBuffer;
	ID3D11Buffer* m_pMaterialBuffer;
	ID3D11Buffer* m_pViewBuffer;
	ID3D11Buffer* m_pLightBuffer;

	ID3D11Resource* m_pResource;

	HWND m_hwnd;

	std::shared_ptr<DirectX::BasicEffect> m_pEffect;
	std::vector<std::shared_ptr<DirectX::SkinnedEffect>> m_pSkinnedEffects;

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

