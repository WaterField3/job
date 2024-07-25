#pragma once

#include <memory>

namespace TMF
{
	class GameObject;
	class Hierarchy
	{
	public:
		Hierarchy() = default;
		virtual ~Hierarchy() = default;

		void DrawImGui();
		inline  std::weak_ptr<GameObject> GetSelectGameObject() const { return m_pSelectGameObject; }

	private:
		std::weak_ptr<GameObject> m_pSelectGameObject;
		 
	};
}