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
			m_pSpriteFont = std::make_unique<DirectX::SpriteFont>(pDevice, L"asset/myfile.spritefont");

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

			const wchar_t* output = L"Hello World";

			DirectX::SimpleMath::Vector2 origin = m_pSpriteFont->MeasureString(output);
			origin /= 2.0f;

			m_pSpriteFont->DrawString(pLockSpriteBatch.get(), output,
				m_spriteFontPos, DirectX::SimpleMath::Color(1.0f,1.0f,1.0f,1.0f), 0.f, origin, 3);

			pLockSpriteBatch->End();
		}
	}
	void Font::OnDrawImGui()
	{
		auto fontPosLabel = StringHelper::CreateLabel("FontPosition", m_uuID);
		if (ImGui::DragFloat2(fontPosLabel.c_str(), &m_spriteFontPos.x))
		{

		}
	}
}