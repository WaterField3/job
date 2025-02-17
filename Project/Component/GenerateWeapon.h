#pragma once
#include "Component.h"

#include "ComponentCerealHelper.h"

namespace TMF
{
	class GameObject;
	class Font;
	class GenerateWeapon : public Component
	{
	public:
		void OnInitialize() override;
		void OnFinalize() override;
		void OnUpdate() override;
		void OnLateUpdate() override;
		void OnDraw() override;
		void OnDrawImGui() override;
		std::shared_ptr<Component> OnClone() const override;
	private:
		void Load();
		void Generate();
	private:
		float m_timer = 0.0f;
		float m_generateRate = 5.0f;
		DirectX::SimpleMath::Vector3 m_generateRange = DirectX::SimpleMath::Vector3::One;
		std::vector<std::shared_ptr<GameObject>> m_pOriginWeapons;
		std::string m_file = "WeaponDatas";
		// �f�o�b�O�p�@�ő���`�F�b�N
		int m_generateCount = 0;
		std::weak_ptr<Font> m_pFont;

		SERIALIZE_COMPONENT(m_file, m_generateRate, m_generateRange);
	};
}
