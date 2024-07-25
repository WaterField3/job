#pragma once

#include <memory>

namespace TMF
{
	class GameObject;
	class Inspector
	{
	public:
		void DrawImGui(std::weak_ptr<GameObject> pGameObject);
	};
}

