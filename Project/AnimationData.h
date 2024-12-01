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

		// �A�j���[�V�����f�[�^�i�[�����i�L�[�̓��[�V�������j
		std::unordered_map<std::string, const aiScene*> m_Animation;

		// importer����������ہ@�V�[������������̂Ń����o�ϐ��ɂ��Ă�
		Assimp::Importer m_importer;
	public:
		AnimationData();
		~AnimationData();

		// �A�j���[�V�����f�[�^�ǂݍ���
		const aiScene* LoadAnimation(const std::string filename, const std::string name);

		// �w�肵�����O�̃A�j���[�V�������擾����
		std::shared_ptr<aiAnimation> GetAnimation(const char* name, int idx = 0);

		// �A�j���[�V�����f�[�^���i�[����Ă��邁���r�����������l������
		inline const aiScene* GetAiScene(std::string name) { return m_Animation[name]; }
	};
}

