#pragma once

#include <memory>

namespace TMF
{
	class Transform;
	class Rigidbody;
	class Dodge;
	class DodgeMove
	{
	public:
		DodgeMove();
		DodgeMove(std::weak_ptr<Transform> pTransform, std::weak_ptr<Rigidbody> pRigidbody, std::weak_ptr<Dodge> pDodge);
		~DodgeMove();

		void DodgeStart();
		void Update();

	private:
		std::weak_ptr<Dodge> m_pDodge;
	};
}

