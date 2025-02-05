#pragma once
#include "WeponBase.h"

#include "ComponentCerealHelper.h"

namespace TMF
{
	class Sword : public WeponBase
	{
	public:
		void OnInitialize() override;
		void OnFinalize() override;
		void OnUpdate() override;
		void OnLateUpdate() override;
		void OnDraw() override;
		void OnDrawImGui() override;
		std::shared_ptr<Component> OnClone() const override;
		void Play() override;
		void Select() override;
		inline bool GetIsMeleeEnd() const { return m_isMeleeEnd; }
		inline bool GetIsMelee() const { return m_isMelee; }
	private:

		bool m_isMelee = false;
		bool m_isMeleeEnd = false;
		std::string m_meleeAnimation = "";

		SERIALIZE_COMPONENT(m_meleeAnimation, m_endTime);
	};
}

