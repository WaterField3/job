#pragma once
#include "UIBase.h"

#include "ComponentCerealHelper.h"

namespace TMF
{
	class Font;
	class WeponBase;
	class WeponUI : public UIBase
	{
	public:
		void OnInitialize() override;
		void OnFinalize() override;
		void OnUpdate() override;
		void OnLateUpdate() override;
		void OnDraw() override;
		void OnDrawImGui() override;
		std::shared_ptr<Component> OnClone() const override;

		void SetSelectWepon(std::weak_ptr<WeponBase> wepon);

	protected:
		virtual void OnSetSelectWepon(std::weak_ptr<WeponBase> wepon);

	protected:
		std::weak_ptr<WeponBase> m_pWepon;

		//SERIALIZE_COMPONENT();
	};
}

