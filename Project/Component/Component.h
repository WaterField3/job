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
		boost::uuids::uuid GetUUID();

		inline bool GetIsEnable() const { return m_isEnable; }
		inline void SetIsEnable(bool set) { m_isEnable = set; }

	protected:
		virtual void OnInitialize();
		virtual void OnFinalize();
		virtual void OnUpdate();
		virtual void OnLateUpdate();
		virtual void OnDraw();
		virtual void OnDrawImGui();
		virtual boost::uuids::uuid OnGetUUID();

		std::weak_ptr<GameObject> m_pOwner;
		bool m_isEnable = true;
	private:
	};
}
