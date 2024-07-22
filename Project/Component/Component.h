#pragma once

#include <memory>

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

	protected:
		virtual void OnInitialize();
		virtual void OnFinalize();
		virtual void OnUpdate();
		virtual void OnLateUpdate();
		virtual void OnDraw();

		std::weak_ptr<GameObject> m_pOwner;

	private:


	};


}

