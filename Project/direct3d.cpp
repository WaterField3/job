
#include "WaveFrontReader.h"
#include <atltypes.h> // CRect���g�����߂̃w�b�_�[�t�@�C��
#include "WICTextureLoader.h" // �e�N�X�`���ǂݍ��݃��C�u����


// �R���p�C���ς݃V�F�[�_�[���C���N���[�h
#include "VertexShader.h"
#include "PixelShader.h"
#include "direct3d.h"

// ���C�u�����̃����N�ݒ�
#pragma comment (lib, "d3d11.lib")

using namespace DirectX;


// �O���[�o���ϐ��̐錾
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

// IASetVertexBuffers�Ŏg�p����ϐ�
const UINT strides = sizeof(D3D::Vertex);
const UINT offsets = 0;

// �֐��̃v���g�^�C�v�錾
HRESULT D3D_Create(HWND hwnd);
void    D3D_Release();


// Direct3D�̏�����������
HRESULT D3D::Create(HWND hwnd)
{
    // HRESULT�^�E�E�EWindows�v���O�����Ŋ֐����s�̐���/���s���󂯎��
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

    // �f�o�C�X�ƃX���b�v�`�F�C���𓯎��ɍ쐬����֐��̌Ăяo��
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
    if (FAILED(hr)) // ��̊֐��Ăяo�������s���ĂȂ���if�Ń`�F�b�N
        return hr;


    ID3D11Texture2D* pBackBuffer;

    hr = m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
    if (FAILED(hr))
        return hr;

    // �����_�[�^�[�Q�b�g���쐬����֐����Ăяo��
    hr = m_pDevice->CreateRenderTargetView(pBackBuffer, NULL, &m_pRenderTargetView);
    pBackBuffer->Release();
    if (FAILED(hr))
        return hr;

    // �[�x�X�e���V���o�b�t�@���쐬
    // ���[�x�o�b�t�@�iZ�o�b�t�@�j�����s�𔻒肵�đO��֌W�𐳂����`��ł���
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

    // �C���v�b�g���C�A�E�g�쐬
    // �����_�P������ɂǂ�ȃf�[�^���܂܂�邩��Direct3D�ɋ�����
    D3D11_INPUT_ELEMENT_DESC g_VertexDesc[]
    {
        // �ʒu���W������Ƃ������Ƃ�`����
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,
            D3D11_INPUT_PER_VERTEX_DATA, 0 },

        // UV���W������Ƃ������Ƃ�`����
        { "TEX",    0, DXGI_FORMAT_R32G32_FLOAT, 0,
            D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },

        // �@���x�N�g��������Ƃ������Ƃ�`����
        { "NORMAL",    0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
            D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };

    hr = m_pDevice->CreateInputLayout(g_VertexDesc, ARRAYSIZE(g_VertexDesc),
        g_vs_main, sizeof(g_vs_main),
        &m_pInputLayout);
    if (FAILED(hr))
        return hr;

    // �V�F�[�_�[�̃I�u�W�F�N�g���쐬
    // �R���p�C���ς݃V�F�[�_�[��VRAM�ɔz�u����GPU�����s�ł���悤�ɂ���
    hr = m_pDevice->CreateVertexShader(&g_vs_main, sizeof(g_vs_main), NULL, &m_pVertexShader);
    if (FAILED(hr))
        return hr;

    hr = m_pDevice->CreatePixelShader(&g_ps_main, sizeof(g_ps_main), NULL, &m_pPixelShader);
    if (FAILED(hr))
        return hr;

    // �r���[�|�[�g���쐬
    // ����ʕ����ȂǂɎg���A�`��̈�̎w��̂���
    m_Viewport.TopLeftX = 0;
    m_Viewport.TopLeftY = 0;
    m_Viewport.Width = (FLOAT)rect.Width();
    m_Viewport.Height = (FLOAT)rect.Height();
    m_Viewport.MinDepth = 0.0f;
    m_Viewport.MaxDepth = 1.0f;
    
    // �T���v���[�쐬
    // ���e�N�X�`���g��k�����̃A���S���Y��
    D3D11_SAMPLER_DESC smpDesc;

    ::ZeroMemory(&smpDesc, sizeof(D3D11_SAMPLER_DESC));
    // �K�p�����t�B���^�[�i�A���S���Y���j�̎��
    smpDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    smpDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    smpDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    smpDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
    hr = m_pDevice->CreateSamplerState(&smpDesc, &m_pSampler);
    if (FAILED(hr)) {
        return hr;
    }

    // �萔�o�b�t�@�쐬
    D3D11_BUFFER_DESC cbDesc;
    cbDesc.ByteWidth = sizeof(ConstBuffer);// ��������萔�o�b�t�@�̃T�C�Y
    cbDesc.Usage = D3D11_USAGE_DEFAULT;
    cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;// �萔�o�b�t�@�쐬���w��
    cbDesc.CPUAccessFlags = 0;
    cbDesc.MiscFlags = 0;
    cbDesc.StructureByteStride = 0;

    hr = m_pDevice->CreateBuffer(&cbDesc, NULL, &m_pConstantBuffer);
   

    // �u�����h�X�e�[�g�쐬
    // �����ߏ�������Z�������\�ɂ���F�̍������@
    // �@���Z�����@���@�G�t�F�N�g�ȂǂɎg�p����F�����邭�Ȃ鍇�����@
    D3D11_BLEND_DESC BlendState;
    ZeroMemory(&BlendState, sizeof(D3D11_BLEND_DESC));
    BlendState.AlphaToCoverageEnable = FALSE;
    BlendState.IndependentBlendEnable = FALSE;
    BlendState.RenderTarget[0].BlendEnable = TRUE;
    BlendState.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
    // �w�i�F�Ɋ|�����킹��W��
    BlendState.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
    BlendState.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
    BlendState.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
    BlendState.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
    BlendState.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
    BlendState.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
    hr = m_pDevice->CreateBlendState(&BlendState, &m_pBlendStateAlpha);

    m_pImmediateContext->OMSetBlendState(m_pBlendStateAlpha, NULL, 0xffffffff);

    // ���Z�����̐ݒ�����
    // �w�i�F�Ɋ|�����킹��W��
    BlendState.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
    m_pDevice->CreateBlendState(&BlendState, &m_pBlendStateAdditive);

    m_hwnd = hwnd;


    return hr;
}

// Direct3D�̉���i�I�������j������
// ��DirectX�̊e�@�\�͍쐬������A�A�v���I�����ɕK��������Ȃ���΂Ȃ�Ȃ�
D3D::~D3D()
{
    if (m_pImmediateContext) {
        m_pImmediateContext->ClearState();
    }

    // �u�����h�X�e�[�g���
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


// �����B�FUV���W�̉E���̒l
D3D::Model D3D::CreateSquare(FLOAT_XY center, FLOAT_XY size, FLOAT_XY uv)
{
    float l = center.x - size.x / 2.0f;
    float r = l + size.x;
    float t = center.y + size.y / 2.0f;
    float b = t - size.y;

    Vertex vertexList[] =
    {
        // ���_�����v���ɎO�p�`���`�����鑤���|���S���̕\�ɂȂ�
        { {l, t, 0.0f}, {0.00f, 0.00f} },  // �O�Ԗڂ̒��_���W�@{ x, y, z, u, v }
        { {r, b, 0.0f}, {uv.x, uv.y} },  // �P�Ԗڂ̒��_���W
        { {l, b, 0.0f}, {0.00f, uv.y} },  // �Q�Ԗڂ̒��_���W

        { {l, t, 0.0f}, {0.00f, 0.00f} },  // �R�Ԗڂ̒��_
        { {r, t, 0.0f}, {uv.x, 0.00f} },  // �S�Ԗڂ̒��_
        { {r, b, 0.0f}, {uv.x, uv.y} },  // �T�Ԗڂ̒��_
    };

    // ���_�o�b�t�@���쐬����
    // �����_�o�b�t�@��VRAM�ɒ��_�f�[�^��u�����߂̋@�\
    D3D11_BUFFER_DESC bufferDesc;
    bufferDesc.ByteWidth = sizeof(vertexList);// �m�ۂ���o�b�t�@�T�C�Y���w��
    bufferDesc.Usage = D3D11_USAGE_DEFAULT;
    bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;// ���_�o�b�t�@�쐬���w��
    bufferDesc.CPUAccessFlags = 0;
    bufferDesc.MiscFlags = 0;
    bufferDesc.StructureByteStride = 0;

    D3D11_SUBRESOURCE_DATA subResourceData;
    subResourceData.pSysMem = vertexList;// VRAM�ɑ���f�[�^���w��
    subResourceData.SysMemPitch = 0;
    subResourceData.SysMemSlicePitch = 0;

    Model model = {};
    // ���_�o�b�t�@�쐬���āA�����ϐ��i��R�����j�ɕۑ�
    Get()->GetDevice()->CreateBuffer(&bufferDesc, &subResourceData, &model.vertexBuffer);

    // ���_�̉��Ԗڂ��g���Ėʂ��\�����邩�Ƃ����f�[�^��
    // �u�C���f�b�N�X�o�b�t�@�v�Ƃ����B
    WORD indices[] = { 0, 1, 2, 3, 4, 5 };
    D3D11_BUFFER_DESC ibDesc;
    ibDesc.ByteWidth = sizeof(indices); //2�o�C�g�~��
    ibDesc.Usage = D3D11_USAGE_DEFAULT;
    ibDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    ibDesc.CPUAccessFlags = 0;
    ibDesc.MiscFlags = 0;
    ibDesc.StructureByteStride = 0;

    D3D11_SUBRESOURCE_DATA irData;
    irData.pSysMem = indices;// �C���f�b�N�X�z��f�[�^���w��
    irData.SysMemPitch = 0;
    irData.SysMemSlicePitch = 0;

    D3D::Get()->GetDevice()->CreateBuffer(
        &ibDesc, &irData, &model.indexBuffer);

    // �C���f�b�N�X����ۑ�
    model.numIndex = sizeof(indices) / sizeof(indices[0]);

    return model;
}

D3D::Model D3D::LoadObjModel(const wchar_t* fileName)
{
    // Todo: Obj�t�@�C����ǂݍ��߂�悤�ɂ���
    // Microsoft��WaveFrontReader�N���X���g����OBJ�t�@�C����ǂݍ���
    WaveFrontReader<uint16_t> reader;

    wchar_t wideName[256];
    wcscpy_s(wideName, L"asset/"); // ������R�s�[�֐�
    wcscat_s(wideName, fileName); // �����񌋍��֐�

    HRESULT hr;
    hr = reader.Load(wideName, true);

    if (FAILED(hr)) // �ǂݍ��ݎ��s������
    {
        MessageBoxA(NULL, "���f���ǂݍ��ݎ��s", "�G���[", MB_OK | MB_ICONERROR);
        return Model(); // ��̃��f����Ԃ�
    }

    // reader�̒��ɓǂݍ��܂ꂽ�f�[�^�𗬂����ޔz��
    // ���_�̌����擾����
    const int numVertex = reader.vertices.size();
    Vertex* vertexList;
    vertexList = new Vertex[numVertex]; // �z��̓��I����

    // ���_�f�[�^�𗬂�����
    for (int i = 0; i < numVertex; i++)
    {
        vertexList[i].position = reader.vertices[i].position;
        vertexList[i].uv = reader.vertices[i].textureCoordinate;
        // �@���x�N�g����OBJ�t�@�C������擾
        vertexList[i].normal = reader.vertices[i].normal;

        // �E����W�n���f����UV�΍�
        vertexList[i].uv.y = 1.0f - vertexList[i].uv.y;

        // �����@���x�N�g�����t�����̃f�[�^�Ȃ甽�]������
        vertexList[i].normal.x *= -1;
        vertexList[i].normal.y *= -1;
        vertexList[i].normal.z *= -1;
    }

    // ���_�o�b�t�@���쐬����
    // �����_�o�b�t�@��VRAM�ɒ��_�f�[�^��u�����߂̋@�\
    D3D11_BUFFER_DESC bufferDesc;
    bufferDesc.ByteWidth = sizeof(Vertex)*numVertex;// �m�ۂ���o�b�t�@�T�C�Y���w��
    bufferDesc.Usage = D3D11_USAGE_DEFAULT;
    bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;// ���_�o�b�t�@�쐬���w��
    bufferDesc.CPUAccessFlags = 0;
    bufferDesc.MiscFlags = 0;
    bufferDesc.StructureByteStride = 0;

    D3D11_SUBRESOURCE_DATA subResourceData;
    subResourceData.pSysMem = vertexList;// VRAM�ɑ���f�[�^���w��
    subResourceData.SysMemPitch = 0;
    subResourceData.SysMemSlicePitch = 0;

    Model model = {};
    // ���_�o�b�t�@�쐬���āA�����ϐ��i��R�����j�ɕۑ�
    Get()->GetDevice()->CreateBuffer(&bufferDesc, &subResourceData, &model.vertexBuffer);

    // ���_�̉��Ԗڂ��g���Ėʂ��\�����邩�Ƃ����f�[�^��
    // �u�C���f�b�N�X�o�b�t�@�v�Ƃ����B
    D3D11_BUFFER_DESC ibDesc;
    ibDesc.ByteWidth = sizeof(WORD) * reader.indices.size(); //2�o�C�g�~��
    ibDesc.Usage = D3D11_USAGE_DEFAULT;
    ibDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    ibDesc.CPUAccessFlags = 0;
    ibDesc.MiscFlags = 0;
    ibDesc.StructureByteStride = 0;

    D3D11_SUBRESOURCE_DATA irData;
    irData.pSysMem = reader.indices.data();// vector.data�֐��Ŕz��f�[�^�擾
    irData.SysMemPitch = 0;
    irData.SysMemSlicePitch = 0;

    hr = D3D::Get()->GetDevice()->CreateBuffer(
        &ibDesc, &irData, &model.indexBuffer);

    // �C���f�b�N�X����ۑ�
    model.numIndex = reader.indices.size();

    // ���I���������z������
    delete[] vertexList;

    // �e�N�X�`���ǂݍ���
    // �t�@�C�������擾���ăt�H���_���ƌ�������
    wchar_t textureName[256];
    wcscpy_s(textureName, L"asset/"); // ������R�s�[�֐�
    wcscat_s(textureName, reader.materials[1].strTexture); // �����񌋍��֐�
    model.texture = D3D::LoadTexture(textureName);

    return model;
}

D3DTEXTURE D3D::LoadTexture(const wchar_t* filename)
{
    D3DTEXTURE texture;

    HRESULT hr;
    // ��S�����F�ǂݍ��񂾃e�N�X�`����ێ�����ϐ�
    hr = DirectX::CreateWICTextureFromFile(Get()->GetDevice(), filename,
        NULL, &texture);

    if (FAILED(hr))
    { // �ǂݍ��݂Ɏ��s�����ꍇ
        MessageBoxA(NULL, "�e�N�X�`���ǂݍ��݂Ɏ��s", "�G���[",
            MB_OK | MB_ICONERROR);
    }

    return texture;
}

D3D* D3D::Get()
{
    static D3D instance; // �B���D3D�N���X�C���X�^���X
    return &instance;
}

DirectX::XMFLOAT3 D3D::RotateVector3(DirectX::XMFLOAT3 srcVector, DirectX::XMFLOAT3 rotation)
{
    // ��]�s��
    // XYZ�̎O���̉�]�p�x���w�肵�ĉ�]��������@�@���@�I�C���[�p
    XMMATRIX matrixRotateY = XMMatrixRotationY(XMConvertToRadians(rotation.y));
    XMMATRIX matrixRotateX = XMMatrixRotationX(XMConvertToRadians(rotation.x));
    XMMATRIX matrixRotateZ = XMMatrixRotationZ(XMConvertToRadians(rotation.z));

    // �R���̉�]���܂Ƃ߂��s��
    XMMATRIX matrixRotate = matrixRotateX * matrixRotateY * matrixRotateZ;

    XMVECTOR vSrcVector = XMVectorSet(srcVector.x, srcVector.y, srcVector.z, 0);

    // �����O�����x�N�g���Ɖ�]�s����|����
    XMVECTOR vRotatedVector
        = XMVector3TransformCoord(vSrcVector, matrixRotate);

    // XMVECTOR��XMFLOAT3�ϊ�
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
    // ��ʓh��Ԃ��F
    float clearColor[4] = { 0.0f, 0.0f, 1.0f, 0.5f }; //red,green,blue,alpha

    // �`���̃L�����o�X�Ǝg�p����[�x�o�b�t�@���w�肷��
    m_pImmediateContext->OMSetRenderTargets(1, &m_pRenderTargetView, m_pDepthStencilView);
    // �`���L�����o�X��h��Ԃ�
    m_pImmediateContext->ClearRenderTargetView(m_pRenderTargetView, clearColor);
    // �[�x�o�b�t�@�����Z�b�g����
    m_pImmediateContext->ClearDepthStencilView(m_pDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

    UINT strides = sizeof(Vertex);
    UINT offsets = 0;
    m_pImmediateContext->IASetInputLayout(m_pInputLayout);

    m_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    m_pImmediateContext->VSSetShader(m_pVertexShader, NULL, 0);
    m_pImmediateContext->RSSetViewports(1, &m_Viewport);
    m_pImmediateContext->PSSetShader(m_pPixelShader, NULL, 0);


    // �s�N�Z���V�F�[�_�[�ɃT���v���[��n��
    m_pImmediateContext->PSSetSamplers(0, 1, &m_pSampler);

    // �萔�o�b�t�@�𒸓_�V�F�[�_�[�ɃZ�b�g����
    m_pImmediateContext->VSSetConstantBuffers(
        0, 1, &m_pConstantBuffer);
    // �萔�o�b�t�@���s�N�Z���V�F�[�_�[�ɃZ�b�g����
    m_pImmediateContext->PSSetConstantBuffers(
        0, 1, &m_pConstantBuffer);
}

void D3D::UpdateScreen()
{
    // �_�u���o�b�t�@�̐؂�ւ����s����ʂ��X�V����
    m_pSwapChain->Present(1, 0);
}
