#pragma once

#include "Component.h"

#include <memory>

#include "ComponentCerealHelper.h"
#include "Model.h"

namespace TMF
{
	class Model;
	class Animation : public Component
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
		//DirectX::ModelBone::TransformArray m_pDrawBone;
		std::weak_ptr<DirectX::Model> m_pModel;

	};
}
