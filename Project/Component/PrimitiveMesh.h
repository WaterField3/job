#pragma once

#include "Component.h"

#include <memory>
#include <GeometricPrimitive.h>
#include <string>
#include <wrl/client.h>

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

    private:
        void LoadTexture();

    private:
        enum ShapeType
        {
            CUBE,
            SPHER,
            CYLINDER,
            CONE,
        };

    private:
        DirectX::SimpleMath::Color m_color = DirectX::SimpleMath::Color(1.0f, 1.0f, 1.0f, 1.0f);
        ShapeType m_shapeType = ShapeType::CUBE;
        std::unique_ptr<DirectX::GeometricPrimitive> m_pShape;
        // ��{�`��̃T�C�Y����{�I��2�ɂ���Ɨǂ�
        DirectX::SimpleMath::Vector3 m_scale = DirectX::SimpleMath::Vector3(2.0f,2.0f,2.0f);
        SERIALIZE_COMPONENT(m_shapeType, m_color, m_scale);
    };
}
