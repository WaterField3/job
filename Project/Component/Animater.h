
#pragma once

#include "Component.h"

#include <memory>

#include "ComponentCerealHelper.h"
#include "DirectXTK/include/Model.h"

namespace DX
{
	class AnimationCMO;
	class AnimationSDKMESH;
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
		void SetFileName(std::string fileName, float endTime);

		void LoadAnimation();

	private:
		void LoadCMO();
		void LoadSDKMESH();

	private:
		bool m_isAnimation = false;
		bool m_isNextAnimSet = false;
		float m_animationSpeed = 1.0f;
		float m_timer = 0;
		float m_animEndTime = 0;
		float m_nextAnimEnd = 0;
		size_t m_animOffset = 0;
		size_t m_boneSize = 0;
		std::string m_fileName = "";
		std::string m_idlePath = "";
		std::string m_nextPath = "";
		std::unique_ptr<DX::AnimationCMO> m_pAnimationCMO;
		std::unique_ptr<DX::AnimationSDKMESH> m_pAnimationSDKMESH;
		DirectX::ModelBone::TransformArray m_drawBone;
		std::weak_ptr<DirectX::Model> m_pModel;


		SERIALIZE_COMPONENT(m_fileName ,m_idlePath, m_animationSpeed, m_isAnimation);
	};
}
