#pragma once
#include "Component.h"

#include <d3d11.h>
#include <wrl.h>

#include "ComponentCerealHelper.h"
#include "MoveInfo.h"

namespace TMF
{
	class Transform;
	class Rigidbody;
	class Thruster : public Component
	{
	public:
		Thruster();
		~Thruster();
		void OnInitialize() override;
		void OnFinalize() override;
		void OnUpdate() override;
		void OnLateUpdate() override;
		void OnDraw() override;
		void OnDrawImGui() override;
		void FastMovement(MoveDirection moveDirection);
		void StopFastMovement();
		void UseThruster(float useMagnification);
		inline bool GetIsOverHeat() const { return m_isOverHeat; }
		inline float GetThrusterValue() const { return m_thrusterValue; }
		inline float GetMaxThrusterValue() const { return m_maxThrusterValue; }
	private:

		bool m_isThruster = false;
		bool m_isOverHeat = false;
		bool m_isDontUseThruster = false;
		float m_firstUseMagnification = 1.0f;
		float m_useMagnification = 0.05f;
		float m_maxThrusterValue = 10.0f;
		float m_thrusterValue = 0.0;
		float m_thrusterMoveSpeedMagnification = 4.0f;
		std::string m_barTextureName = "asset/textures/sunset.jpg";
		DirectX::SimpleMath::Vector3 m_moveVector = DirectX::SimpleMath::Vector3::Zero;
		DirectX::SimpleMath::Vector2 m_drawUIPosition = DirectX::SimpleMath::Vector2::Zero;
		MoveDirection m_moveDirection = MoveDirection::FOWARD;
		std::weak_ptr<Transform> m_pTransform;
		std::weak_ptr<Rigidbody> m_pRigidBody;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_pBarTexture;
		SERIALIZE_COMPONENT(m_thrusterValue, m_firstUseMagnification, m_useMagnification,m_thrusterMoveSpeedMagnification, m_barTextureName, m_drawUIPosition);
	};
}