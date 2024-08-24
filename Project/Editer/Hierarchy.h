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
		void DrawTree(std::weak_ptr<Transform> pTransform);
		void TransformDropTarget(const char* dragDropLabel, std::weak_ptr<TMF::Transform> pTransform);
		std::vector<std::weak_ptr<Transform>> GetTransformChildren(std::weak_ptr<Transform> pTransform);
	private:
		std::weak_ptr<GameObject> m_pSelectGameObject;
		std::vector<std::weak_ptr<Transform>> m_pTransformsCache;
	};
}
