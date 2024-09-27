#pragma once

#include <Effekseer/Effekseer/Effekseer.h>
#include <Effekseer/EffekseerRendererDX11/EffekseerRendererDX11.h>
#include <memory>
#include <SimpleMath.h>

namespace TMF
{
	class EffectManager
	{
	public:
		void Initialize();
		void Finalize();
		void Update();
		void Draw();
		void Play(std::string name, DirectX::SimpleMath::Vector3 pos, DirectX::SimpleMath::Matrix matrix = DirectX::SimpleMath::Matrix::Identity);

		static EffectManager& Instance()
		{
			static EffectManager instance;
			return instance;
		}

	private:
		void CopyMatrix(const DirectX::SimpleMath::Matrix& tkMatrix, Effekseer::Matrix44& efMatrix);
		void CopyMatrix(const DirectX::SimpleMath::Matrix& tkMatrix, Effekseer::Matrix43& efMatrix);

	private:
		Effekseer::ManagerRef m_pEffekseerManager;
		EffekseerRendererDX11::RendererRef m_pEffectRenderer;
		Effekseer::EffectRef m_pEffect;
	};
}
