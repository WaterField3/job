#pragma once

#include <memory>

#include <string>

namespace TMF
{
	class GameObject;
	class Inspector
	{
	public:
		void DrawImGui(std::weak_ptr<GameObject> pGameObject);

	private:
	};
}
