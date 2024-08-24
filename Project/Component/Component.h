#pragma once

#include <memory>
#include <string>
#include <cereal/types/base_class.hpp>
#include <boost/uuid/uuid.hpp>

namespace TMF
{
	class GameObject;
	class Component
	{
	public:
		Component();
		virtual ~Component();

		template<typename Archive>
		void serialize(Archive& ar)
		{
			return;
		}

		void Initialize(std::weak_ptr<GameObject> pOwner);
		void Finalize();
		void Update();
		void LateUpdate();
		void Draw();
		void DrawImGui();
		void CollisionEnter(GameObject* pGameObject);
		void CollisionStay(GameObject* pGameObject);
		void CollisionExit(GameObject* pGameObject);
		void TrigerEnter(GameObject* pGameObject);
		void TrigerStay(GameObject* pGameObject);
		void TrigerExit(GameObject* pGameObject);
		boost::uuids::uuid GetUUID();

		// Remove‰Â”\‚©
		inline virtual bool IsRemovable() { return true; }
		inline bool GetIsEnable() const { return m_isEnable; }
		inline void SetIsEnable(bool set) { m_isEnable = set; }

	protected:
		virtual void OnInitialize();
		virtual void OnFinalize();
		virtual void OnUpdate();
		virtual void OnLateUpdate();
		virtual void OnDraw();
		virtual void OnDrawImGui();
		virtual void OnCollisionEnter(GameObject* pGameObject);
		virtual void OnCollisionStay(GameObject* pGameObject);
		virtual void OnCollisionExit(GameObject* pGameObject);
		virtual void OnTrigerEnter(GameObject* pGameObject);
		virtual void OnTrigerStay(GameObject* pGameObject);
		virtual void OnTrigerExit(GameObject* pGameObject);
		virtual boost::uuids::uuid OnGetUUID();

	protected:
		std::weak_ptr<GameObject> m_pOwner;
		bool m_isEnable = true;
	private:
	};
}
