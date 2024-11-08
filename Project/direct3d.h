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

#include "RenderTexture.h"
#include "SpriteBatch.h"
#include "VertexTypes.h"

// Direct3D解放の簡略化マクロ
#define SAFE_RELEASE(p)      { if( NULL != p ) { p->Release(); p = NULL; } }

// 既存の型名を別名でも使えるようにする
typedef ID3D11ShaderResourceView* D3DTEXTURE;
typedef DirectX::XMFLOAT2 FLOAT_XY;

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

	// 頂点データ１つあたりのバイトサイズを返す
	UINT GetVertexStride();
	void PostProcess();
	inline BloomPresets GetBloomPresets() { return g_Bloom; }
	void SetBloomPresets(BloomPresets set);
private:
	D3D() {};


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

	ID3D11Resource* m_pResource;

	HWND m_hwnd;

	std::shared_ptr<DirectX::BasicEffect> m_pEffect;

	std::unique_ptr<DirectX::SpriteBatch> m_pSpriteBatch;

	std::shared_ptr<DirectX::CommonStates> m_pCommonStates;

	Microsoft::WRL::ComPtr <ID3D11ShaderResourceView> m_pBackGround;

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

