#pragma once
#include "Component.h"

#include <string>
#include <memory>
#include <map>

#include "ComponentCerealHelper.h"
#include "BoneCombMatrix.h"
#include "AnimationMesh.h"
#include "AnimationData.h"
#include "Shader.h"

namespace TMF
{
	class AssimpAnimation : public Component
	{
	public:
		void OnInitialize() override;
		void OnFinalize() override;
		void OnUpdate() override;
		void OnLateUpdate() override;
		void OnDraw() override;
		void OnDrawImGui() override;

	private:

		// SRT情報
		//DirectX::SimpleMath::Vector3 m_Position = { 0,0,0 };
		//DirectX::SimpleMath::Vector3 m_Rotation = { 0,0,0 };
		//DirectX::SimpleMath::Vector3 m_Scale = { 0.1f,0.1f,0.1f };

		// 現在フレーム
		float m_CurrentFrame = 0;

		// ブレンドレイト
		float m_BlendRate = 0.0f;									// モーションのブレンド率

		Shader m_shader;

		// ボーンコンビネーション行列用定数バッファ内容
		BoneCombMatrix m_BoneCombMatrix{};							// 20240723

		// アニメーションメッシュ
		std::unique_ptr<AnimationMesh> m_AnimMesh;

		// アニメーションデータ
		std::vector<std::shared_ptr<AnimationData>> m_animationData;

		// モデルファイル
		std::string m_currentFile = "asset/model/Ch36_nonPBR.fbx";

		// テクスチャファイル
		std::string m_textureDirectory = "asset/model";

		std::string m_motionFile = "asset/model/Idle.fbx";

		// 現在のアニメーション
		std::shared_ptr<aiAnimation> m_CurrentAnimation;
		std::shared_ptr<aiAnimation> m_FromAnimation;
		std::shared_ptr<aiAnimation> m_ToAnimation;

	public:

		void SetAnimationMesh(std::unique_ptr<AnimationMesh> animmesh) { m_AnimMesh = std::move(animmesh); }
		//void SetPosition(DirectX::SimpleMath::Vector3 pos) { m_Position = pos; }
		//void SetScale(DirectX::SimpleMath::Vector3 scale) { m_Scale = scale; }
		//void SetRotation(DirectX::SimpleMath::Vector3 rotation) { m_Rotation = rotation; }

		void SetCurrentAnimation(std::shared_ptr<aiAnimation> anim) { m_CurrentAnimation = anim; }

		void SetFromAnimation(std::shared_ptr<aiAnimation> anim) { m_FromAnimation = anim; }

		void SetToAnimation(std::shared_ptr<aiAnimation> anim) { m_ToAnimation = anim; }

		void SetBlendRate(float rate) { m_BlendRate = rate; }
	};
}

