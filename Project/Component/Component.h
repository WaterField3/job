#pragma once

#include <memory>
#include <string>

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

	protected:
		virtual void OnInitialize();
		virtual void OnFinalize();
		virtual void OnUpdate();
		virtual void OnLateUpdate();
		virtual void OnDraw();
		virtual void OnDrawImGui();

		std::weak_ptr<GameObject> m_pOwner;

	private:


	};


}

