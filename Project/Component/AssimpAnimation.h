#pragma once
#include "Component.h"

#include <string>
#include <memory>
#include <map>

#include "ComponentCerealHelper.h"
#include "BoneCombMatrix.h"
#include "AnimationMesh.h"
#include "AnimationData.h"

namespace TMF
{
	class Transform;
	class AssimpAnimation : public Component
	{
	public:
		void OnInitialize() override;
		void OnFinalize() override;
		void OnUpdate() override;
		void OnLateUpdate() override;
		void OnDraw() override;
		void OnDrawImGui() override;
		std::shared_ptr<Component> OnClone() const override;

		inline void SetAnimationMesh(std::unique_ptr<AnimationMesh> animmesh) { m_AnimMesh = std::move(animmesh); }

		inline void SetCurrentAnimation(std::shared_ptr<aiAnimation> anim) { m_CurrentAnimation = anim; }

		inline void SetFromAnimation(std::shared_ptr<aiAnimation> anim) { m_FromAnimation = anim; }

		inline void SetToAnimation(std::shared_ptr<aiAnimation> anim) { m_ToAnimation = anim; }

		inline void SetBlendRate(float rate) { m_BlendRate = rate; }
	private:

		// SRT情報
		//DirectX::SimpleMath::Vector3 m_Position = { 0,0,0 };
		//DirectX::SimpleMath::Vector3 m_Rotation = { 0,0,0 };
		//DirectX::SimpleMath::Vector3 m_Scale = { 0.1f,0.1f,0.1f };

		// 現在フレーム
		float m_CurrentFrame = 0;

		// ブレンドレイト
		float m_BlendRate = 1.0f;									// モーションのブレンド率

		// ボーンコンビネーション行列用定数バッファ内容
		BoneCombMatrix m_BoneCombMatrix{};							// 20240723

		// アニメーションメッシュ
		std::unique_ptr<AnimationMesh> m_AnimMesh;

		// アニメーションデータ
		std::vector<std::shared_ptr<AnimationData>> m_animationData;

		// モデルファイル
		std::string m_currentFile = "asset/model/03.fbx";

		// テクスチャファイル
		std::string m_textureDirectory = "asset/model";

		std::string m_motionFile = "asset/model/03Idle.fbx";

		std::weak_ptr<Transform> m_pTransform;

		// 現在のアニメーション
		std::shared_ptr<aiAnimation> m_CurrentAnimation;
		std::shared_ptr<aiAnimation> m_FromAnimation;
		std::shared_ptr<aiAnimation> m_ToAnimation;
	};
}

