
#pragma once

#include "Component.h"

#include <memory>

#include "ComponentCerealHelper.h"
#include "Model.h"

namespace DX
{
	class AnimationCMO;
}

namespace TMF
{
	class Model;
	class Animater : public Component
	{
	public:
		void OnInitialize() override;
		void OnFinalize() override;
		void OnUpdate() override;
		void OnLateUpdate() override;
		void OnDraw() override;
		void OnDrawImGui() override;

	private:
		float m_animOffset = 0;
		int m_boneSize = 0;
		std::string m_fileName = "";
		//DX::AnimationCMO m_animation;
		std::unique_ptr<DX::AnimationCMO> m_pAnimation;
		DirectX::ModelBone::TransformArray m_drawBone;
		std::weak_ptr<DirectX::Model> m_pModel;


		SERIALIZE_COMPONENT(m_fileName);
	};
}
