#pragma once

#include "Component.h"

#include "ComponentCerealHelper.h"

namespace TMF
{
	class Fade;
	class SceneLoad : public Component
	{
	public:
		void OnInitialize() override;
		void OnFinalize() override;
		void OnUpdate() override;
		void OnLateUpdate() override;
		void OnDraw() override;
		void OnDrawImGui() override;
		std::shared_ptr<Component> OnClone() const override;
		void Load();

	private:
		enum SceneChangeTrigger
		{
			FADEIN,
			FADEOUT,
		};
		std::string m_loadFile = "";
		SceneChangeTrigger m_trigger;
		std::weak_ptr<Fade> m_pFade;
		SERIALIZE_COMPONENT(m_trigger, m_loadFile);
	};
}
