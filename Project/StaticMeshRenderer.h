#pragma once
#include "MeshRenderer.h"

#include	"StaticMesh.h"
#include	"AssimpTexture.h"
#include    "AssimpMaterial.h"
#include "direct3d.h"

namespace TMF
{
	class StaticMeshRenderer : public MeshRenderer
	{
		std::vector<D3D::SUBSET> m_Subsets;
	std::vector<std::unique_ptr<AssimpTexture>> m_DiffuseTextures;
	std::vector<std::shared_ptr<AssimpMaterial>> m_Materiales;
public:	
	void Init(StaticMesh& mesh);
	void Draw();
	};
}

