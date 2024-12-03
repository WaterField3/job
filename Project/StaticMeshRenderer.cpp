#include "StaticMeshRenderer.h"

namespace TMF
{
	void StaticMeshRenderer::Init(StaticMesh& mesh)
	{
		// 頂点バッファとインデックスバッファを生成
		MeshRenderer::Init(mesh);

		// サブセット情報取得
		m_Subsets = mesh.GetSubsets();

		// diffuseテクスチャ情報取得

		m_DiffuseTextures = mesh.GetDiffuseTextures();

		// マテリアル情報取得	
		std::vector<D3D::MATERIAL> materials;
		materials = mesh.GetMaterials();

		// マテリアル数分ループしてマテリアルデータを生成
		for (int i = 0; i < materials.size(); i++)
		{
			// マテリアルオブジェクト生成
			std::unique_ptr<AssimpMaterial> m = std::make_unique<AssimpMaterial>();

			// マテリアル情報をセット
			m->Create(materials[i]);

			// マテリアルオブジェクトを配列に追加
			m_Materiales.push_back(std::move(m));
		}
	}
	void StaticMeshRenderer::Draw()
	{
		// インデックスバッファ・頂点バッファをセット
		BeforeDraw();

		// マテリアル数分ループ 
		for (int i = 0; i < m_Subsets.size(); i++)
		{
			// マテリアルをセット(サブセット情報の中にあるマテリアルインデックを使用する)
			m_Materiales[m_Subsets[i].MaterialIdx]->SetGPU();

			if (m_Materiales[m_Subsets[i].MaterialIdx]->isDiffuseTextureEnable())
			{
				m_DiffuseTextures[m_Subsets[i].MaterialIdx]->SetGPU();
			}

			// サブセットの描画
			DrawSubset(
				m_Subsets[i].IndexNum,							// 描画するインデックス数
				m_Subsets[i].IndexBase,							// 最初のインデックスバッファの位置	
				m_Subsets[i].VertexBase);						// 頂点バッファの最初から使用
		}
	}
}
