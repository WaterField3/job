#include "Model.h"

#include <Imgui/imgui.h>

namespace TMF
{
	REGISTER_COMPONENT(Model);

	Model::Model()
	{

	}

	Model::~Model()
	{

	}

	void Model::OnInitialize()
	{

	}

	void Model::OnFinalize()
	{

	}

	void Model::OnUpdate()
	{

	}

	void Model::OnLateUpdate()
	{

	}

	void Model::OnDraw()
	{

	}

	void Model::OnDrawImGui()
	{
		char buf[256] = "";
		strcpy_s(buf, sizeof(buf), m_loadFileName.c_str());
		if (ImGui::InputText("fileName", buf, 256))
		{
			m_loadFileName = buf;
		}
	}
}
