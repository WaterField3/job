#include "AnimationData.h"

#include	<cassert>
#include	<iostream>

namespace TMF
{
	AnimationData::AnimationData()
	{
	}
	AnimationData::~AnimationData()
	{
		for (auto anim : m_Animation)
		{
			//delete anim.second;
		}
	}
	const aiScene* AnimationData::LoadAnimation(const std::string filename, const std::string name)
	{
		//Assimp::Importer importer;



		//importer.ReadFile(filename.c_str(), aiProcess_ConvertToLeftHanded);

		//auto orphanedAiScene = importer.GetOrphanedScene();
		//m_Animation.emplace(name, orphanedAiScene);
		//assert(m_Animation[name]);

		try
		{
			// ÉVÅ[ÉìèÓïÒÇç\íz
			m_Animation[name] = m_importer.ReadFile(
				filename.c_str(),
				aiProcess_ConvertToLeftHanded);
			assert(m_Animation[name]);

		}
		catch (const std::exception&)
		{

		}

		if (m_Animation[name] == nullptr) {
			std::cout << " animation load error " << filename << " " << m_importer.GetErrorString();
		}

		return std::move(m_Animation[name]);
	}
	std::shared_ptr<aiAnimation> AnimationData::GetAnimation(const char* name, int idx)
	{
		auto anim = m_Animation[name]->mAnimations[idx];
		return move(std::shared_ptr<aiAnimation>(anim));
	}
}
