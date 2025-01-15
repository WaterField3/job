#pragma once

#include "Component.h"

#include "ComponentCerealHelper.h"
#include "DirectXTK/include/Model.h"

namespace TMF
{
	class Transform;
	class Model;
	class Animater;
	class BoneBind : public Component
	{
	public:
		void OnInitialize() override;
		void OnFinalize() override;
		void OnUpdate() override;
		void OnLateUpdate() override;
		void OnDraw() override;
		void OnDrawImGui() override;

	private:
		bool m_isBindBone = false;
		std::string m_bindName = "";
		size_t m_boneSize = 0;
		DirectX::ModelBone::TransformArray m_drawBones;
		DirectX::ModelBone::TransformArray m_animBones;
		std::weak_ptr<Transform> m_pTransform;
		std::weak_ptr<Transform> m_pParent;
		std::weak_ptr<DirectX::DX11::Model> m_pModel;
		std::weak_ptr<Animater> m_pAnimater;
		//SERIALIZE_COMPONENT();
	};
}

