#pragma once
#include "Mesh.h"
class BoxMesh :
    public Mesh
{
    void Normalize(DirectX::XMFLOAT3 _vector, DirectX::XMFLOAT3& _normal);
    void CreateIndex();
    void CreateVertex();

public:

    void Initialize(DirectX::XMFLOAT3 _scale, DirectX::XMFLOAT4 _color);
    void Draw();


private:
    DirectX::XMFLOAT4 m_color;
    DirectX::XMFLOAT3 m_length;
    // épê®ç¿ïWÇÃèÓïÒ
    DirectX::XMFLOAT3 m_position = DirectX::XMFLOAT3(0.0f, 10.0f, 0.0f);
    DirectX::XMFLOAT3 m_rotation = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
    DirectX::XMFLOAT3 m_scale = DirectX::XMFLOAT3(10.0f, 10.0f, 10.0f);
};

