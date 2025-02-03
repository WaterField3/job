#pragma once
#include "Component.h"

#include "ComponentCerealHelper.h"

namespace TMF
{
	class Transform;
	class Damage : public Component
	{
	public:
		void OnInitialize() override;
		void OnFinalize() override;
		void OnUpdate() override;
		void OnLateUpdate() override;
		void OnDraw() override;
		void OnDrawImGui() override;
		void OnTrigerEnter(GameObject* pGameObject) override;
		void OnCollisionEnter(GameObject* pGameObject) override;
		std::shared_ptr<Component> OnClone() const override;

		inline void SetDamageValue(float setDamage) { m_damage = setDamage; }
	private:
		enum DamageReactionType
		{
			STAGGER,
			INVERT,
			NONE,
		};
		DamageReactionType m_reactionType = NONE;
		std::weak_ptr<Transform> m_pTransform;
		std::weak_ptr<Transform> m_pParent;
		float m_damage = 1.0f;
		SERIALIZE_COMPONENT(m_damage, m_reactionType);
	};
}

