#pragma once

#include "Component/Component.h"

#include <cereal/types/polymorphic.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <d3d11.h>
#include <SimpleMath.h>
#include <string>

#include "Component/ComponentCerealHelper.h"
#include "Component/ComponentRegister.h"
#include "direct3d.h"

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
		boost::uuids::uuid OnGetUUID() override;

	private:
		void ModelDraw();

		std::string m_loadFileName = "NoSetting";
		boost::uuids::uuid m_uuID = boost::uuids::random_generator()();
		D3D::Model m_model;
		bool m_isDraw = true;

		SERIALIZE_COMPONENT(m_loadFileName,m_isDraw, m_uuID);
	};
}
REGISTER_CEREAL_TYPE(TMF::Model);
