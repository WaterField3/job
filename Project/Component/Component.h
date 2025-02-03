#pragma once

#include <memory>
#include <string>
#include <cereal/types/base_class.hpp>

// windowsのmin maxを使用しないようにする
#undef min
#undef max

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_io.hpp>
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
		std::shared_ptr<Component> Clone() const;

		// Remove可能か
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
		/// <summary>
		/// 同じデータの新しいコンポーネントを作成
		/// </summary>
		/// <returns>各コンポーネントのShared_ptr</returns>
		virtual std::shared_ptr<Component> OnClone() const;

		template <typename TComponent>
		std::shared_ptr<TComponent> GetComponent(TComponent component)
		{
			if (auto pLockOwner = m_pOwner.lock())
			{

			}
		}

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
			try{
				archive(::cereal::make_nvp("m_isEnable", m_isEnable));
			}
			catch (const std::exception&){
			}
		};
	};
}
