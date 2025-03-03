#pragma once

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/cimport.h>

#include "AssimpTexture.h"
#include "TreeNode.h"

namespace TMF
{
	class AssimpPerse
	{
	public:
		AssimpPerse() {};
		~AssimpPerse() = default;

		struct VERTEX {
			std::string meshname;				// メッシュ名
			aiVector3D	pos;					// 位置
			aiVector3D	normal;					// 法線
			aiColor4D	color;					// 頂点カラー	
			aiVector3D	texcoord;				// テクスチャ座標	
			int			materialindex;			// マテリアルインデックス
			std::string mtrlname;				// マテリアル名

			int			BoneIndex[4];			// ボーンインデックス
			float		BoneWeight[4];			// ボーンウェイト
			std::string	BoneName[4];			// ボーン名
			int			bonecnt = 0;			// ボーン数
		};

		struct SUBSET {
			std::string meshname;				// メッシュ名
			int materialindex;					// マテリアルインデックス
			unsigned int VertexBase;			// 頂点バッファのベース
			unsigned int VertexNum;				// 頂点数
			unsigned int IndexBase;				// インデックスバッファのベース
			unsigned int IndexNum;				// インデックス数
			std::string	 mtrlname;				// マテリアル名
		};

		struct MATERIAL {
			std::string mtrlname;					// マテリアル名
			aiColor4D	Ambient;					// アンビエント
			aiColor4D	Diffuse;					// ディフューズ
			aiColor4D	Specular;					// スペキュラ
			aiColor4D	Emission;					// エミッション
			float		Shiness;					// シャイネス
			std::string diffusetexturename;			// ディフューズテクスチャ名
		};

		// ウェイト情報
		struct WEIGHT {
			std::string bonename;						// ボーン名
			std::string meshname;						// メッシュ名
			float weight;								// ウェイト値
			int	vertexindex;							// 頂点インデックス
		};

		//ボーン構造体
		struct BONE
		{
			std::string bonename;						// ボーン名
			std::string meshname;						// メッシュ名
			std::string armaturename;					// アーマチュア名
			aiMatrix4x4 Matrix{};						// 親子関係を考慮した行列
			aiMatrix4x4 AnimationMatrix{};				// 自分の事だけを考えた行列
			aiMatrix4x4 OffsetMatrix{};					// ボーンオフセット行列
			int			idx;							// 配列の何番目か			
			std::vector<WEIGHT> weights;				// このボーンが影響を与える頂点インデックス・ウェイト値
		};

		static AssimpPerse& Instance()
		{
			static AssimpPerse instance;
			return instance;
		}
		void GetModelData(std::string filename, std::string texturedirectory);
		std::vector<SUBSET> GetSubsets();
		std::vector<std::vector<VERTEX>> GetVertices();
		std::vector<std::vector<unsigned int>> GetIndices();
		std::vector<MATERIAL> GetMaterials();
		std::vector<std::unique_ptr<AssimpTexture>> MoveDiffuseTextures();
		std::unordered_map<std::string, BONE> GetBoneDictionary();
		std::shared_ptr<TreeNode<std::string>> GetBoneNameTree();

		std::vector<std::vector<VERTEX>> m_vertices{};				// 頂点データ（メッシュ単位）
		std::vector<std::vector<unsigned int>> m_indices{};			// インデックスデータ（メッシュ単位）
		std::vector<SUBSET> m_subsets{};							// サブセット情報
		std::vector<MATERIAL> m_materials{};						// マテリアル
		std::vector<std::unique_ptr<AssimpTexture>> m_diffuseTextures;	// ディフューズテクスチャ群
		std::unordered_map<std::string, BONE> m_BoneDictionary;		// ボーン辞書（キー：ボーン名）
		std::vector<std::vector<BONE>>	m_BonesPerMeshes;			// メッシュ単位でボーン情報を集めたもの
		std::shared_ptr<TreeNode<std::string>>	m_AssimpNodeNameTree;				// assimpノード名ツリー

	private:
		void GetMaterialData(const aiScene* pScene, std::string texturedirectory);
		std::vector<BONE> GetBonesPerMesh(const aiMesh* mesh);
		void CreateEmptyBoneDictionary(aiNode* node);
		void SetBoneDataToVertices();
		void CreateNodeTree(aiNode* node, std::weak_ptr<TreeNode<std::string>> ptree);
		void GetBone(const aiScene* pScene);

	};
}

