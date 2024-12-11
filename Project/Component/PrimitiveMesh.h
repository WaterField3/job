#pragma once

#include "Component.h"

#include <memory>
#include <GeometricPrimitive.h>
#include <string>
#include <wrl/client.h>
#include <d3d11.h>

#include "ComponentCerealHelper.h"

namespace TMF
{
    class PrimitiveMesh : public Component
    {
    public:
        void OnInitialize() override;
        void MakeMesh();
        void OnFinalize() override;
        void OnUpdate() override;
        void OnLateUpdate() override;
        void OnDraw() override;
        void OnDrawImGui() override;
        void SetColor(DirectX::SimpleMath::Color color);

    private:
        void LoadTexture();

    private:
        enum ShapeType
        {
            CUBE,
            SPHER,
            CYLINDER,
            CONE,
            TORUS,
        };

    private:
        bool m_isSetDepthBufferMode = false;
        bool m_isInvertn = false;
        bool m_isRhcoords = true;
        std::string m_textureName = "";
        Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_pTexture;
        DirectX::SimpleMath::Color m_color = DirectX::SimpleMath::Color(1.0f, 1.0f, 1.0f, 1.0f);
        ShapeType m_shapeType = ShapeType::CUBE;
        std::unique_ptr<DirectX::GeometricPrimitive> m_pShape;
        // 基本形状のサイズ※基本的に2にすると良い
        DirectX::SimpleMath::Vector3 m_scale = DirectX::SimpleMath::Vector3(2.0f,2.0f,2.0f);
        SERIALIZE_COMPONENT(m_isInvertn, m_isRhcoords, m_shapeType, m_color, m_scale, m_textureName);
    };
}
