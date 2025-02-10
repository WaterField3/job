#include "GenerateWepon.h"

#include <cereal/archives/json.hpp>
#include <cereal/cereal.hpp>
#include <fstream>
#include <filesystem>
#include <Imgui/imgui.h>
#include <random>

#include "GameObject/GameObject.h"
#include "GameObject/GameObjectManager.h"
#include "Utility/StringHelper.h"
#include "ComponentRegister.h"
#include "Transform.h"
#include "Font.h"
#include "Timer.h"

REGISTER_COMPONENT(TMF::GenerateWepon, "GenarateWepon");

namespace TMF
{
	void GenerateWepon::OnInitialize()
	{
		if (auto pLockOwner = m_pOwner.lock())
		{
			auto pFont = pLockOwner->GetComponent<Font>();
			if (auto pLockFont = pFont.lock())
			{
				m_pFont = pLockFont;
			}
		}
		Load();
	}
	void GenerateWepon::OnFinalize()
	{
	}
	void GenerateWepon::OnUpdate()
	{
		if (auto pLockFont = m_pFont.lock())
		{
			auto str = std::to_string(m_generateCount);
			pLockFont->SetText(str);
		}

		Generate();
	}
	void GenerateWepon::OnLateUpdate()
	{
	}
	void GenerateWepon::OnDraw()
	{
	}
	void GenerateWepon::OnDrawImGui()
	{
		auto generateRateLabel = StringHelper::CreateLabel("GenerateRate", m_uuID);
		if (ImGui::DragFloat(generateRateLabel.c_str(), &m_generateRate))
		{

		}

		auto generateRangeLabel = StringHelper::CreateLabel("GenerateRange", m_uuID);
		if (ImGui::DragFloat3(generateRangeLabel.c_str(), &m_generateRange.x))
		{

		}
		if (ImGui::Button("Generate"))
		{
			Generate();
		}
	}
	std::shared_ptr<Component> GenerateWepon::OnClone() const
	{
		auto pClone = std::make_shared<GenerateWepon>();
		pClone->m_file = this->m_file;
		return move(pClone);
	}
	void GenerateWepon::Load()
	{
		auto fileName = m_file + ".json";
		if (!std::filesystem::is_regular_file(fileName))
		{
			return;
		}
		std::ifstream iS(fileName.c_str(), std::ios::in);
		{
			cereal::JSONInputArchive inArchive(iS);
			inArchive(m_pOriginWepons);
		}
	}
	void GenerateWepon::Generate()
	{
		//if (Timer::Instance().fps < 55.0f)
		//{
		//	return;
		//}
		auto deltaTime = Timer::Instance().deltaTime.count();
		m_timer += deltaTime;
		if (m_generateRate < m_timer)
		{
			auto size = static_cast<int>(m_pOriginWepons.size());
			if (size > 0)
			{
				--size;
			}
			std::random_device randomDevice;
			std::mt19937 gen(randomDevice());

			std::uniform_int_distribution<int> dist(0, size);

			auto selectIndex = dist(gen);
			while (selectIndex == 0)
			{
				selectIndex = dist(gen);
			}
			auto pGenerateObject = GameObjectManager::Instance().CreateGameObject(m_pOriginWepons[selectIndex]);
			if (auto pLockGenerateObject = pGenerateObject.lock())
			{
				auto pGenerateTransform = pLockGenerateObject->GetComponent<Transform>();
				if (auto pLockGenerateTransform = pGenerateTransform.lock())
				{
					std::uniform_real_distribution<float> dist_x(-m_generateRange.x, m_generateRange.x);
					std::uniform_real_distribution<float> dist_y(0, m_generateRange.y);
					std::uniform_real_distribution<float> dist_z(-m_generateRange.z, m_generateRange.z);
					auto randomPosition = DirectX::SimpleMath::Vector3(dist_x(gen), dist_y(gen), dist_z(gen));
					pLockGenerateTransform->SetPosition(randomPosition);
				}
			}
			m_generateCount++;
			m_timer = 0.0f;
		}
	}
}
