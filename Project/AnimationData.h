#pragma once

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/cimport.h>
#include <unordered_map>
#include <string>
#include <memory>

namespace TMF
{
	class AnimationData
	{
		std::string m_filename;

		// アニメーションデータ格納辞書（キーはモーション名）
		std::unordered_map<std::string, const aiScene*> m_Animation;

		// importerが解放される際　シーンも解放されるのでメンバ変数にしてる
		Assimp::Importer m_importer;
	public:
		AnimationData();
		~AnimationData();

		// アニメーションデータ読み込み
		const aiScene* LoadAnimation(const std::string filename, const std::string name);

		// 指定した名前のアニメーションを取得する
		std::shared_ptr<aiAnimation> GetAnimation(const char* name, int idx = 0);

		// アニメーションデータが格納されているａｉＳｃｅｎｅを獲得する
		inline const aiScene* GetAiScene(std::string name) { return m_Animation[name]; }
	};
}

