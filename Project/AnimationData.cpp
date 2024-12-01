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
	}
	const aiScene* AnimationData::LoadAnimation(const std::string filename, const std::string name)
	{

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

		return m_Animation[name];
	}
	std::shared_ptr<aiAnimation> AnimationData::GetAnimation(const char* name, int idx)
	{
		auto anim = m_Animation[name]->mAnimations[idx];
		std::shared_ptr<aiAnimation> animation = std::shared_ptr<aiAnimation>(anim);
		return animation;
	}
}
