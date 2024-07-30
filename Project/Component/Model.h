#pragma once

#include "Component/Component.h"

#include <cereal/types/polymorphic.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <SimpleMath.h>
#include <string>

#include "Component/ComponentCerealHelper.h"
#include "Component/ComponentRegister.h"

namespace TMF
{
	class Model : public Component
	{
	public:
		Model();
		~Model();
		void OnInitialize() override;
		void OnFinalize() override;
		void OnUpdate() override;
		void OnLateUpdate() override;
		void OnDraw() override;
		void OnDrawImGui() override;

	private:
		std::string m_loadFileName = "NoSetting";
		boost::uuids::uuid m_uuID = boost::uuids::random_generator()();

		SERIALIZE_COMPONENT(m_loadFileName, m_uuID);
	};
}
REGISTER_CEREAL_TYPE(TMF::Model);
