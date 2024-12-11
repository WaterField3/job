#include "Font.h"

#include <Imgui/imgui.h>

#include "Utility/Log.h"
#include "Utility/StringHelper.h"
#include "direct3d.h"
#include "ComponentRegister.h"

REGISTER_COMPONENT(TMF::Font, "Font");

namespace TMF
{
	void Font::OnInitialize()
	{
		auto pDevice = D3D::Get()->GetDevice();
		try
		{
			if (m_fontPath == "")
			{
				return;
			}
			auto wStringPath = std::wstring(m_fontPath.begin(), m_fontPath.end());
			m_pSpriteFont = std::make_unique<DirectX::SpriteFont>(pDevice, wStringPath.c_str());

		}
		catch (const std::exception& e)
		{
			Log::Info("%s", e.what());
		}
		m_pSpriteBatch = D3D::Get()->GetSpriteBatch();
	}
	void Font::OnFinalize()
	{
		m_pSpriteFont.reset();
	}
	void Font::OnUpdate()
	{
	}
	void Font::OnLateUpdate()
	{
	}
	void Font::OnDraw()
	{
		if (auto pLockSpriteBatch = m_pSpriteBatch.lock())
		{
			pLockSpriteBatch->Begin();

			const char* output = m_text.c_str();

			DirectX::SimpleMath::Vector2 origin = m_pSpriteFont->MeasureString(output);
			origin /= 2.0f;

			m_pSpriteFont->DrawString(pLockSpriteBatch.get(), output,
				m_spriteFontPos, m_fontColor, 0.f, origin, m_fontScale);

			pLockSpriteBatch->End();
		}
	}
	void Font::OnDrawImGui()
	{
		auto fontPosLabel = StringHelper::CreateLabel("FontPosition", m_uuID);
		if (ImGui::DragFloat2(fontPosLabel.c_str(), &m_spriteFontPos.x))
		{

		}

		auto fontScaleLabel = StringHelper::CreateLabel("FontScale", m_uuID);
		if (ImGui::DragFloat(fontScaleLabel.c_str(), &m_fontScale, 0.1f))
		{

		}

		auto fontColorLabel = StringHelper::CreateLabel("FonstColor", m_uuID);
		if (ImGui::DragFloat3(fontColorLabel.c_str(), &m_fontColor.x, 0.1f, 0.0f, 1.0f))
		{

		}

		auto textLabel = StringHelper::CreateLabel("Text", m_uuID);
		char buf[256] = "";
		strcpy_s(buf, sizeof(buf), m_text.c_str());
		if (ImGui::InputText(textLabel.c_str(), buf, 256))
		{
			m_text = buf;
		}

		auto fontPathLabel = StringHelper::CreateLabel("FontPath", m_uuID);
		char fontBuf[256] = "";
		strcpy_s(fontBuf, sizeof(fontBuf), m_fontPath.c_str());
		if (ImGui::InputText(fontPathLabel.c_str(), fontBuf, 256))
		{
			m_fontPath = fontBuf;
		}

		auto loadLabel = StringHelper::CreateLabel("Load", m_uuID);
		if (ImGui::Button(loadLabel.c_str()))
		{
			if (m_fontPath == "")
			{
				return;
			}
			auto pDevice = D3D::Get()->GetDevice();
			auto wStringPath = std::wstring(m_fontPath.begin(), m_fontPath.end());
			m_pSpriteFont = std::make_unique<DirectX::SpriteFont>(pDevice, wStringPath.c_str());
		}
	}
}