
#include "WaveFrontReader.h"
#include <atltypes.h> // CRectを使うためのヘッダーファイル
#include "WICTextureLoader.h" // テクスチャ読み込みライブラリ


// コンパイル済みシェーダーをインクルード
#include "VertexShader.h"
#include "PixelShader.h"
#include "direct3d.h"

// ライブラリのリンク設定
#pragma comment (lib, "d3d11.lib")

using namespace DirectX;


// グローバル変数の宣言
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

// IASetVertexBuffersで使用する変数
const UINT strides = sizeof(D3D::Vertex);
const UINT offsets = 0;

// 関数のプロトタイプ宣言
HRESULT D3D_Create(HWND hwnd);
void    D3D_Release();


// Direct3Dの初期化をする
HRESULT D3D::Create(HWND hwnd)
{
    // HRESULT型・・・Windowsプログラムで関数実行の成功/失敗を受け取る
    HRESULT  hr;


    UINT flags = 0;
//#ifdef _DEBUG
//    flags |= D3D11_CREATE_DEVICE_DEBUG;
//#endif
    D3D_FEATURE_LEVEL pLevels[] = { D3D_FEATURE_LEVEL_11_0 };
    D3D_FEATURE_LEVEL level;

    CRect                rect;
    DXGI_SWAP_CHAIN_DESC scDesc;

    ::GetClientRect(hwnd, &rect);
    ::ZeroMemory(&scDesc, sizeof(scDesc));
    scDesc.BufferCount = 1;
    scDesc.BufferDesc.Width = rect.Width();
    scDesc.BufferDesc.Height = rect.Height();
    scDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    scDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    scDesc.OutputWindow = hwnd;
    scDesc.SampleDesc.Count = 1;
    scDesc.SampleDesc.Quality = 0;
    scDesc.Windowed = TRUE;

    // デバイスとスワップチェインを同時に作成する関数の呼び出し
    hr = D3D11CreateDeviceAndSwapChain(NULL,
        D3D_DRIVER_TYPE_HARDWARE,
        NULL,
        flags,
        pLevels,
        1,
        D3D11_SDK_VERSION,
        &scDesc,
        &m_pSwapChain,
        &m_pDevice,
        &level,
        &m_pImmediateContext);
    if (FAILED(hr)) // 上の関数呼び出しが失敗してないかifでチェック
        return hr;


    ID3D11Texture2D* pBackBuffer;

    hr = m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
    if (FAILED(hr))
        return hr;

    // レンダーターゲットを作成する関数を呼び出す
    hr = m_pDevice->CreateRenderTargetView(pBackBuffer, NULL, &m_pRenderTargetView);
    pBackBuffer->Release();
    if (FAILED(hr))
        return hr;

    // 深度ステンシルバッファを作成
    // ※深度バッファ（Zバッファ）→奥行を判定して前後関係を正しく描画できる
    D3D11_TEXTURE2D_DESC txDesc;
    ZeroMemory(&txDesc, sizeof(txDesc));
    txDesc.Width = rect.Width();
    txDesc.Height = rect.Height();
    txDesc.MipLevels = 1;
    txDesc.ArraySize = 1;
    txDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    txDesc.SampleDesc.Count = 1;
    txDesc.SampleDesc.Quality = 0;
    txDesc.Usage = D3D11_USAGE_DEFAULT;
    txDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    txDesc.CPUAccessFlags = 0;
    txDesc.MiscFlags = 0;
    hr = m_pDevice->CreateTexture2D(&txDesc, NULL, &m_pDepthStencilTexture);
    if (FAILED(hr))
        return hr;

    D3D11_DEPTH_STENCIL_VIEW_DESC dsDesc;
    ZeroMemory(&dsDesc, sizeof(dsDesc));
    dsDesc.Format = txDesc.Format;
    dsDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
    dsDesc.Texture2D.MipSlice = 0;
    hr = m_pDevice->CreateDepthStencilView(m_pDepthStencilTexture, &dsDesc, &m_pDepthStencilView);
    if (FAILED(hr))
        return hr;

    // インプットレイアウト作成
    // →頂点１つあたりにどんなデータが含まれるかをDirect3Dに教える
    D3D11_INPUT_ELEMENT_DESC g_VertexDesc[]
    {
        // 位置座標があるということを伝える
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,
            D3D11_INPUT_PER_VERTEX_DATA, 0 },

        // UV座標があるということを伝える
        { "TEX",    0, DXGI_FORMAT_R32G32_FLOAT, 0,
            D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },

        // 法線ベクトルがあるということを伝える
        { "NORMAL",    0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
            D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };

    hr = m_pDevice->CreateInputLayout(g_VertexDesc, ARRAYSIZE(g_VertexDesc),
        g_vs_main, sizeof(g_vs_main),
        &m_pInputLayout);
    if (FAILED(hr))
        return hr;

    // シェーダーのオブジェクトを作成
    // コンパイル済みシェーダーをVRAMに配置してGPUが実行できるようにする
    hr = m_pDevice->CreateVertexShader(&g_vs_main, sizeof(g_vs_main), NULL, &m_pVertexShader);
    if (FAILED(hr))
        return hr;

    hr = m_pDevice->CreatePixelShader(&g_ps_main, sizeof(g_ps_main), NULL, &m_pPixelShader);
    if (FAILED(hr))
        return hr;

    // ビューポートを作成
    // →画面分割などに使う、描画領域の指定のこと
    m_Viewport.TopLeftX = 0;
    m_Viewport.TopLeftY = 0;
    m_Viewport.Width = (FLOAT)rect.Width();
    m_Viewport.Height = (FLOAT)rect.Height();
    m_Viewport.MinDepth = 0.0f;
    m_Viewport.MaxDepth = 1.0f;
    
    // サンプラー作成
    // →テクスチャ拡大縮小時のアルゴリズム
    D3D11_SAMPLER_DESC smpDesc;

    ::ZeroMemory(&smpDesc, sizeof(D3D11_SAMPLER_DESC));
    // 適用されるフィルター（アルゴリズム）の種類
    smpDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    smpDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    smpDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    smpDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
    hr = m_pDevice->CreateSamplerState(&smpDesc, &m_pSampler);
    if (FAILED(hr)) {
        return hr;
    }

    // 定数バッファ作成
    D3D11_BUFFER_DESC cbDesc;
    cbDesc.ByteWidth = sizeof(ConstBuffer);// 今から作る定数バッファのサイズ
    cbDesc.Usage = D3D11_USAGE_DEFAULT;
    cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;// 定数バッファ作成を指定
    cbDesc.CPUAccessFlags = 0;
    cbDesc.MiscFlags = 0;
    cbDesc.StructureByteStride = 0;

    hr = m_pDevice->CreateBuffer(&cbDesc, NULL, &m_pConstantBuffer);
   

    // ブレンドステート作成
    // →透過処理や加算合成を可能にする色の合成方法
    // 　加算合成　→　エフェクトなどに使用する色が明るくなる合成方法
    D3D11_BLEND_DESC BlendState;
    ZeroMemory(&BlendState, sizeof(D3D11_BLEND_DESC));
    BlendState.AlphaToCoverageEnable = FALSE;
    BlendState.IndependentBlendEnable = FALSE;
    BlendState.RenderTarget[0].BlendEnable = TRUE;
    BlendState.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
    // 背景色に掛け合わせる係数
    BlendState.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
    BlendState.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
    BlendState.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
    BlendState.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
    BlendState.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
    BlendState.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
    hr = m_pDevice->CreateBlendState(&BlendState, &m_pBlendStateAlpha);

    m_pImmediateContext->OMSetBlendState(m_pBlendStateAlpha, NULL, 0xffffffff);

    D3D11_RASTERIZER_DESC rastraizerDesc;
    ZeroMemory(&rastraizerDesc, sizeof(D3D11_RASTERIZER_DESC));
    rastraizerDesc.CullMode = D3D11_CULL_FRONT;
    rastraizerDesc.FillMode = D3D11_FILL_SOLID;

    ID3D11RasterizerState* RasterrizeState;

    hr = m_pDevice->CreateRasterizerState(&rastraizerDesc, &RasterrizeState);
    m_pImmediateContext->RSSetState(RasterrizeState);

    // 加算合成の設定を作る
    // 背景色に掛け合わせる係数
    BlendState.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
    m_pDevice->CreateBlendState(&BlendState, &m_pBlendStateAdditive);


    m_hwnd = hwnd;


    return hr;
}

// Direct3Dの解放（終了処理）をする
// ※DirectXの各機能は作成した後、アプリ終了時に必ず解放しなければならない
D3D::~D3D()
{
    if (m_pImmediateContext) {
        m_pImmediateContext->ClearState();
    }

    // ブレンドステート解放
    SAFE_RELEASE(m_pBlendStateAdditive);
    SAFE_RELEASE(m_pBlendStateAlpha);

    SAFE_RELEASE(m_pConstantBuffer);

    SAFE_RELEASE(m_pSampler);

    SAFE_RELEASE(m_pPixelShader);
    SAFE_RELEASE(m_pVertexShader);
    SAFE_RELEASE(m_pInputLayout);

    SAFE_RELEASE(m_pDepthStencilTexture);
    SAFE_RELEASE(m_pDepthStencilView);
    SAFE_RELEASE(m_pRenderTargetView);
    SAFE_RELEASE(m_pSwapChain);
    SAFE_RELEASE(m_pImmediateContext);
    SAFE_RELEASE(m_pDevice);
}


// 引数③：UV座標の右下の値
D3D::Model D3D::CreateSquare(FLOAT_XY center, FLOAT_XY size, FLOAT_XY uv)
{
    float l = center.x - size.x / 2.0f;
    float r = l + size.x;
    float t = center.y + size.y / 2.0f;
    float b = t - size.y;

    Vertex vertexList[] =
    {
        // 頂点が時計回りに三角形を形成する側がポリゴンの表になる
        { {l, t, 0.0f}, {0.00f, 0.00f} },  // ０番目の頂点座標　{ x, y, z, u, v }
        { {r, b, 0.0f}, {uv.x, uv.y} },  // １番目の頂点座標
        { {l, b, 0.0f}, {0.00f, uv.y} },  // ２番目の頂点座標

        { {l, t, 0.0f}, {0.00f, 0.00f} },  // ３番目の頂点
        { {r, t, 0.0f}, {uv.x, 0.00f} },  // ４番目の頂点
        { {r, b, 0.0f}, {uv.x, uv.y} },  // ５番目の頂点
    };

    // 頂点バッファを作成する
    // ※頂点バッファ→VRAMに頂点データを置くための機能
    D3D11_BUFFER_DESC bufferDesc;
    bufferDesc.ByteWidth = sizeof(vertexList);// 確保するバッファサイズを指定
    bufferDesc.Usage = D3D11_USAGE_DEFAULT;
    bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;// 頂点バッファ作成を指定
    bufferDesc.CPUAccessFlags = 0;
    bufferDesc.MiscFlags = 0;
    bufferDesc.StructureByteStride = 0;

    D3D11_SUBRESOURCE_DATA subResourceData;
    subResourceData.pSysMem = vertexList;// VRAMに送るデータを指定
    subResourceData.SysMemPitch = 0;
    subResourceData.SysMemSlicePitch = 0;

    Model model = {};
    // 頂点バッファ作成して、それを変数（第３引数）に保存
    Get()->GetDevice()->CreateBuffer(&bufferDesc, &subResourceData, &model.vertexBuffer);

    // 頂点の何番目を使って面を構成するかというデータを
    // 「インデックスバッファ」という。
    WORD indices[] = { 0, 1, 2, 3, 4, 5 };
    D3D11_BUFFER_DESC ibDesc;
    ibDesc.ByteWidth = sizeof(indices); //2バイト×個数
    ibDesc.Usage = D3D11_USAGE_DEFAULT;
    ibDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    ibDesc.CPUAccessFlags = 0;
    ibDesc.MiscFlags = 0;
    ibDesc.StructureByteStride = 0;

    D3D11_SUBRESOURCE_DATA irData;
    irData.pSysMem = indices;// インデックス配列データを指定
    irData.SysMemPitch = 0;
    irData.SysMemSlicePitch = 0;

    D3D::Get()->GetDevice()->CreateBuffer(
        &ibDesc, &irData, &model.indexBuffer);

    // インデックス数を保存
    model.numIndex = sizeof(indices) / sizeof(indices[0]);

    return model;
}

D3D::Model D3D::LoadObjModel(const wchar_t* fileName)
{
    // Todo: Objファイルを読み込めるようにする
    // MicrosoftのWaveFrontReaderクラスを使ってOBJファイルを読み込む
    WaveFrontReader<uint16_t> reader;

    wchar_t wideName[256];
    wcscpy_s(wideName, L"asset/"); // 文字列コピー関数
    wcscat_s(wideName, fileName); // 文字列結合関数

    HRESULT hr;
    hr = reader.Load(wideName, true);

    if (FAILED(hr)) // 読み込み失敗したら
    {
        MessageBoxA(NULL, "モデル読み込み失敗", "エラー", MB_OK | MB_ICONERROR);
        return Model(); // 空のモデルを返す
    }

    // readerの中に読み込まれたデータを流し込む配列
    // 頂点の個数を取得する
    const int numVertex = reader.vertices.size();
    Vertex* vertexList;
    vertexList = new Vertex[numVertex]; // 配列の動的生成

    // 頂点データを流し込む
    for (int i = 0; i < numVertex; i++)
    {
        vertexList[i].position = reader.vertices[i].position;
        vertexList[i].uv = reader.vertices[i].textureCoordinate;
        // 法線ベクトルをOBJファイルから取得
        vertexList[i].normal = reader.vertices[i].normal;

        // 右手座標系モデルのUV対策
        vertexList[i].uv.y = 1.0f - vertexList[i].uv.y;

        // もし法線ベクトルが逆向きのデータなら反転させる
        vertexList[i].normal.x *= -1;
        vertexList[i].normal.y *= -1;
        vertexList[i].normal.z *= -1;
    }

    // 頂点バッファを作成する
    // ※頂点バッファ→VRAMに頂点データを置くための機能
    D3D11_BUFFER_DESC bufferDesc;
    bufferDesc.ByteWidth = sizeof(Vertex)*numVertex;// 確保するバッファサイズを指定
    bufferDesc.Usage = D3D11_USAGE_DEFAULT;
    bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;// 頂点バッファ作成を指定
    bufferDesc.CPUAccessFlags = 0;
    bufferDesc.MiscFlags = 0;
    bufferDesc.StructureByteStride = 0;

    D3D11_SUBRESOURCE_DATA subResourceData;
    subResourceData.pSysMem = vertexList;// VRAMに送るデータを指定
    subResourceData.SysMemPitch = 0;
    subResourceData.SysMemSlicePitch = 0;

    Model model = {};
    // 頂点バッファ作成して、それを変数（第３引数）に保存
    Get()->GetDevice()->CreateBuffer(&bufferDesc, &subResourceData, &model.vertexBuffer);

    // 頂点の何番目を使って面を構成するかというデータを
    // 「インデックスバッファ」という。
    D3D11_BUFFER_DESC ibDesc;
    ibDesc.ByteWidth = sizeof(WORD) * reader.indices.size(); //2バイト×個数
    ibDesc.Usage = D3D11_USAGE_DEFAULT;
    ibDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    ibDesc.CPUAccessFlags = 0;
    ibDesc.MiscFlags = 0;
    ibDesc.StructureByteStride = 0;

    D3D11_SUBRESOURCE_DATA irData;
    irData.pSysMem = reader.indices.data();// vector.data関数で配列データ取得
    irData.SysMemPitch = 0;
    irData.SysMemSlicePitch = 0;

    hr = D3D::Get()->GetDevice()->CreateBuffer(
        &ibDesc, &irData, &model.indexBuffer);

    // インデックス数を保存
    model.numIndex = reader.indices.size();

    // 動的生成した配列を解放
    delete[] vertexList;

    // テクスチャ読み込み
    // ファイル名を取得してフォルダ名と結合する
    wchar_t textureName[256];
    wcscpy_s(textureName, L"asset/"); // 文字列コピー関数
    wcscat_s(textureName, reader.materials[1].strTexture); // 文字列結合関数
    model.texture = D3D::LoadTexture(textureName);

    return model;
}

D3DTEXTURE D3D::LoadTexture(const wchar_t* filename)
{
    D3DTEXTURE texture;

    HRESULT hr;
    // 第４引数：読み込んだテクスチャを保持する変数
    hr = DirectX::CreateWICTextureFromFile(Get()->GetDevice(), filename,
        NULL, &texture);

    if (FAILED(hr))
    { // 読み込みに失敗した場合
        MessageBoxA(NULL, "テクスチャ読み込みに失敗", "エラー",
            MB_OK | MB_ICONERROR);
    }

    return texture;
}

D3D* D3D::Get()
{
    static D3D instance; // 唯一のD3Dクラスインスタンス
    return &instance;
}

DirectX::XMFLOAT3 D3D::RotateVector3(DirectX::XMFLOAT3 srcVector, DirectX::XMFLOAT3 rotation)
{
    // 回転行列
    // XYZの三軸の回転角度を指定して回転させる方法　＝　オイラー角
    XMMATRIX matrixRotateY = XMMatrixRotationY(XMConvertToRadians(rotation.y));
    XMMATRIX matrixRotateX = XMMatrixRotationX(XMConvertToRadians(rotation.x));
    XMMATRIX matrixRotateZ = XMMatrixRotationZ(XMConvertToRadians(rotation.z));

    // ３軸の回転をまとめた行列
    XMMATRIX matrixRotate = matrixRotateX * matrixRotateY * matrixRotateZ;

    XMVECTOR vSrcVector = XMVectorSet(srcVector.x, srcVector.y, srcVector.z, 0);

    // 初期前向きベクトルと回転行列を掛ける
    XMVECTOR vRotatedVector
        = XMVector3TransformCoord(vSrcVector, matrixRotate);

    // XMVECTOR→XMFLOAT3変換
    XMFLOAT3 rotatedVector;
    rotatedVector.x = XMVectorGetX(vRotatedVector);
    rotatedVector.y = XMVectorGetY(vRotatedVector);
    rotatedVector.z = XMVectorGetZ(vRotatedVector);

    return rotatedVector;
}

UINT D3D::GetVertexStride()
{
    return sizeof(Vertex);
}

void D3D::ClearScreen()
{
    // 画面塗りつぶし色
    float clearColor[4] = { 0.0f, 0.0f, 1.0f, 0.5f }; //red,green,blue,alpha

    // 描画先のキャンバスと使用する深度バッファを指定する
    m_pImmediateContext->OMSetRenderTargets(1, &m_pRenderTargetView, m_pDepthStencilView);
    // 描画先キャンバスを塗りつぶす
    m_pImmediateContext->ClearRenderTargetView(m_pRenderTargetView, clearColor);
    // 深度バッファをリセットする
    m_pImmediateContext->ClearDepthStencilView(m_pDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

    UINT strides = sizeof(Vertex);
    UINT offsets = 0;
    m_pImmediateContext->IASetInputLayout(m_pInputLayout);

    m_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    m_pImmediateContext->VSSetShader(m_pVertexShader, NULL, 0);
    m_pImmediateContext->RSSetViewports(1, &m_Viewport);
    m_pImmediateContext->PSSetShader(m_pPixelShader, NULL, 0);


    // ピクセルシェーダーにサンプラーを渡す
    m_pImmediateContext->PSSetSamplers(0, 1, &m_pSampler);

    // 定数バッファを頂点シェーダーにセットする
    m_pImmediateContext->VSSetConstantBuffers(
        0, 1, &m_pConstantBuffer);
    // 定数バッファをピクセルシェーダーにセットする
    m_pImmediateContext->PSSetConstantBuffers(
        0, 1, &m_pConstantBuffer);
}

void D3D::UpdateScreen()
{
    // ダブルバッファの切り替えを行い画面を更新する
    m_pSwapChain->Present(1, 0);
}
