#pragma once

#include "Component.h"

#include <cereal/types/polymorphic.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <d3d11.h>
#include <SimpleMath.h>
#include <string>
#include <Model.h>
#include <CommonStates.h>
#include "Effects.h"

#include "ComponentCerealHelper.h"
#include "ComponentRegister.h"
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

	private:
		void ModelDraw();
		void LoadCMO();

		std::string m_loadCmo = "asset/Gun.cmo";
		//D3D::Model m_model;

		std::unique_ptr<DirectX::EffectFactory> m_pEffectFactory;
		std::unique_ptr<DirectX::Model> m_pModel;
		std::unique_ptr<DirectX::CommonStates> m_pCommonState;
		bool m_isDraw = true;

		SERIALIZE_COMPONENT(m_loadCmo, m_isDraw, m_uuID);
	};
}
