#pragma once

#include <memory>
#include <vector>

namespace TMF
{
	class GameObject;
	class Transform;
	class Hierarchy
	{
	public:
		Hierarchy() = default;
		virtual ~Hierarchy() = default;

		void DrawImGui();
		inline  std::weak_ptr<GameObject> GetSelectGameObject() const { return m_pSelectGameObject; }

	private:
		void DrawTree(const Transform* pTransform);
		std::vector<Transform*> GetTransformChildren(const Transform* pTransform);
	private:
		std::weak_ptr<GameObject> m_pSelectGameObject;
		std::vector<Transform*> m_pTransformsCache;
	};
}
