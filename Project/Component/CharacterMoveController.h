#pragma once

#include "Component.h"

#include <cereal/types/polymorphic.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <d3d11.h>
#include <SimpleMath.h>
#include <string>

#include "ComponentCerealHelper.h"
#include "ComponentRegister.h"

namespace TMF
{
	class CharacterMoveController : public Component
	{
	public:
		CharacterMoveController();
		~CharacterMoveController();
		void OnInitialize() override;
		void OnFinalize() override;
		void OnUpdate() override;
		void OnLateUpdate() override;
		void OnDraw() override;
		void OnDrawImGui() override;

	private:
		float m_moveSpeed = 1;
		float m_jumpPower = 1;

		SERIALIZE_COMPONENT(m_moveSpeed, m_uuID);
	};
}
