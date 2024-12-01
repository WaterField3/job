#pragma once

#include <Windows.h>
#include <directxmath.h>
#include <d3d11.h>
#include <wrl/client.h>

#include "dx11helper.h"
#include "direct3d.h"

namespace TMF
{
	class AssimpMaterial
	{
		struct ConstantBufferMaterial
		{
			D3D::MATERIAL Material;
		};

		D3D::MATERIAL m_Material{};
		Microsoft::WRL::ComPtr<ID3D11Buffer> m_pConstantBufferMaterial;
public:
	AssimpMaterial() {}
	AssimpMaterial(D3D::MATERIAL mtrl);

	~AssimpMaterial();

	bool Create(D3D::MATERIAL mtrl);

	void Update();

	void SetGPU();

	void SetMaterial(const D3D::MATERIAL& mtrl);

	void Finalize();

	inline void SetDiffuse(DirectX::XMFLOAT4 diffuse) { m_Material.Diffuse = diffuse;}

	inline void SetAmbient(DirectX::XMFLOAT4 ambient) { m_Material.Ambient = ambient;}

	inline void SetSpecular(DirectX::XMFLOAT4 specular) { m_Material.Specular = specular;}

	inline void SetEmission(DirectX::XMFLOAT4 emission) { m_Material.Emission = emission;}

	inline void SetShiness(float shiness) { m_Material.Shiness = shiness;}

	inline bool isDiffuseTextureEnable() { return m_Material.TextureEnable == TRUE;}
	};
}

