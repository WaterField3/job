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
		enum LOADTYPE
		{
			DEFAULT,
			CMO,
			SDKMESH,
		};

	public:
		Model();
		~Model();
		void OnInitialize() override;
		void OnFinalize() override;
		void OnUpdate() override;
		void OnLateUpdate() override;
		void OnDraw() override;
		void OnDrawImGui() override;
		inline std::weak_ptr<DirectX::Model> GetModel() { return m_pModel; }
		inline size_t GetAnimOffset() const { return animOffset; }
		inline LOADTYPE GetLoadType() const { return m_loadType; }

	private:
		void ModelDraw();
		void LoadCMO();
		void LoadSdkMesh();

	private:
		bool m_isDraw = true;
		bool m_isUseWorldMatrix = false;
		size_t animOffset = 0;
		std::string m_loadFile = "asset/Gun.cmo";
		std::string m_loadDirectory = "asset/model";
		LOADTYPE m_loadType = LOADTYPE::DEFAULT;
		std::unique_ptr<DirectX::EffectFactory> m_pEffectFactory;
		std::shared_ptr<DirectX::Model> m_pModel;
		std::unique_ptr<DirectX::CommonStates> m_pCommonState;

		SERIALIZE_COMPONENT(m_loadFile, m_isDraw, m_loadType, m_loadDirectory);
	};
}
