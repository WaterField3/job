#pragma once

#include <memory>
#include <string>
#include <cereal/types/base_class.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/random_generator.hpp>

#include "Utility/CerealHelper.h"

namespace TMF
{
	class GameObject;
	class Component
	{
	public:
		Component();
		virtual ~Component();

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

		// Remove‰Â”\‚©
		inline virtual bool IsRemovable() { return true; }
		inline bool GetIsEnable() const { return m_isEnable; }
		inline void SetIsEnable(bool set) { m_isEnable = set; }
		inline boost::uuids::uuid GetUUID() const { return m_uuID; }
		inline std::weak_ptr<GameObject> GetOwner() const { return m_pOwner; }

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


	protected:
		std::weak_ptr<GameObject> m_pOwner;
		bool m_isEnable = true;
		boost::uuids::uuid m_uuID = boost::uuids::random_generator()();
	private:
		friend class cereal::access; template<typename TArchive> void serialize(TArchive& archive) {
			try {
				archive(::cereal::make_nvp("m_uuID", m_uuID));
			}
			catch (const std::exception&) {
			}
		};
	};
}
