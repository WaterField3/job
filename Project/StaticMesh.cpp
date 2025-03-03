#include "StaticMesh.h"

#include "AssimpPerse.h"
#include "direct3d.h"

namespace TMF
{
	StaticMesh::StaticMesh()
	{
	}
	StaticMesh::~StaticMesh()
	{
	}
	void StaticMesh::Load(std::string fileName, std::string textureDirectory)
	{
		std::vector<AssimpPerse::SUBSET> subsets{};							// サブセット情報
		std::vector<std::vector<AssimpPerse::VERTEX>> vertices{};			// 頂点データ（メッシュ単位）
		std::vector<std::vector<unsigned int>> indices{};					// インデックスデータ（メッシュ単位）
		std::vector<AssimpPerse::MATERIAL> materials{};						// マテリアル
		std::vector<std::unique_ptr<AssimpTexture>> embededtextures{};			// 内蔵テクスチャ群

		// assimpを使用してモデルデータを取得
		AssimpPerse::Instance().GetModelData(fileName, textureDirectory);

		subsets = AssimpPerse::Instance().GetSubsets();								// サブセット情報取得
		vertices = AssimpPerse::Instance().GetVertices();								// 頂点データ（メッシュ単位）
		indices = AssimpPerse::Instance().GetIndices();								// インデックスデータ（メッシュ単位）
		materials = AssimpPerse::Instance().GetMaterials();							// マテリアル情報取得
		

		//m_diffuseTextures.clear();

		auto data = AssimpPerse::Instance().MoveDiffuseTextures();
		auto size = data.size();
		
		m_diffuseTextures.clear();
		m_diffuseTextures.resize(size);

		for (auto index = 0; index < data.size(); index++)
		{
			m_diffuseTextures[index] = std::move(data[0]);
		}

		// 頂点データ作成
		int meshidx = 0;

		for (const auto& mv : vertices)
		{
			for (auto& v : mv)
			{
				D3D::VERTEX_3D vertex{};
				vertex.Position = DirectX::SimpleMath::Vector3(v.pos.x, v.pos.y, v.pos.z);
				vertex.Normal = DirectX::SimpleMath::Vector3(v.normal.x, v.normal.y, v.normal.z);
				vertex.TexCoord = DirectX::SimpleMath::Vector2(v.texcoord.x, v.texcoord.y);
				vertex.Diffuse = DirectX::SimpleMath::Color(v.color.r, v.color.g, v.color.b, v.color.a);

				vertex.bonecnt = v.bonecnt;
				for (int i = 0; i < 4; i++)
				{
					vertex.BoneIndex[i] = 0;
					vertex.BoneWeight[i] = 0.0f;
					//				vertex.BoneName[i] = "";
				}

				for (int i = 0; i < v.bonecnt; i++)
				{
					vertex.BoneIndex[i] = v.BoneIndex[i];
					vertex.BoneWeight[i] = v.BoneWeight[i];
					//				vertex.BoneName[i] = v.BoneName[i];
				}

				m_vertices.push_back(vertex);
			}
		}

		// インデックスデータ作成
		for (const auto& mi : indices)
		{
			for (auto& index : mi)
			{
				m_indices.emplace_back(index);
			}
		}

		// サブセットデータ作成
		for (const auto& sub : subsets)
		{
			D3D::SUBSET subset{};
			subset.VertexBase = sub.VertexBase;
			subset.VertexNum = sub.VertexNum;
			subset.IndexBase = sub.IndexBase;
			subset.IndexNum = sub.IndexNum;
			subset.MtrlName = sub.mtrlname;
			subset.MaterialIdx = sub.materialindex;					//	マテリアル配列のインデックス
			m_subsets.emplace_back(subset);
		}

		// マテリアルデータ作成(表示のための)
		for (const auto& m : materials)
		{
			D3D::MATERIAL material{};
			material.Ambient = DirectX::SimpleMath::Color(m.Ambient.r, m.Ambient.g, m.Ambient.b, m.Ambient.a);
			material.Diffuse = DirectX::SimpleMath::Color(m.Diffuse.r, m.Diffuse.g, m.Diffuse.b, m.Diffuse.a);
			material.Specular = DirectX::SimpleMath::Color(m.Specular.r, m.Specular.g, m.Specular.b, m.Specular.a);
			material.Emission = DirectX::SimpleMath::Color(m.Emission.r, m.Emission.g, m.Emission.b, m.Emission.a);
			material.Shiness = m.Shiness;
			if (m.diffusetexturename.empty())
			{
				material.TextureEnable = FALSE;
				m_diffuseTextureNames.emplace_back("");
			}
			else
			{
				material.TextureEnable = TRUE;
				m_diffuseTextureNames.emplace_back(m.diffusetexturename);
			}

			m_materials.emplace_back(material);
		}
	}
}