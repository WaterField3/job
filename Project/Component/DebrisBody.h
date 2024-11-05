#pragma once
#include "Component.h"

#include "Component/ComponentCerealHelper.h"

namespace TMF
{
	class DebrisBody : public Component
	{
	public:
		void OnInitialize() override;
		void OnFinalize() override;
		void OnUpdate() override;
		void OnLateUpdate() override;
		void OnDraw() override;
		void OnDrawImGui() override;
		void Play();

	private:
		//SERIALIZE_COMPONENT();
	};
}

