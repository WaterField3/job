#pragma once

#include "Mesh.h"

#include <vector>
#include <SimpleMath.h>
#include <string>
#include <memory>

#include "AssimpTexture.h"
#include "direct3d.h"

namespace TMF
{
	class StaticMesh : public Mesh
	{
	public:
		StaticMesh();
		~StaticMesh();

		static StaticMesh& Instance()
		{
			static StaticMesh instance;
			return instance;
		}

	void Load(std::string fileName, std::string textureDirectory="");

	const std::vector<D3D::MATERIAL>& GetMaterials() {
		return m_materials;
	}

	const std::vector<D3D::SUBSET>& GetSubsets() {
		return m_subsets;
	}

	const std::vector<std::string>& GetDiffuseTextureNames() {
		return m_diffuseTextureNames;
	}

	std::vector<std::unique_ptr<AssimpTexture>> GetDiffuseTextures() {
		return std::move(m_diffuseTextures);
	}

private:

	std::vector<D3D::MATERIAL> m_materials;					// マテリアル情報
	std::vector<std::string> m_diffuseTextureNames;		// ディフューズテクスチャ名
	std::vector<D3D::SUBSET> m_subsets;						// サブセット情報

	std::vector<int> a;

	std::vector<std::unique_ptr<AssimpTexture>>	m_diffuseTextures;	// ディフューズテクスチャ群

	};
}

