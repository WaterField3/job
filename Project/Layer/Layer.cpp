#include "Layer.h"

namespace TMF
{

	void Layer::OnInitialize()
	{

	}

	void Layer::OnFinalize()
	{

	}

	void Layer::OnUpdate()
	{

	}

	void Layer::OnDraw()
	{

	}
	void Layer::SetName(const char name[64])
	{
		m_name = name;
	}
	const char* Layer::GetName()
	{
		return m_name;
	}
	/// <summary>
	/// ���W�n�̒l���擾
	/// </summary>
	/// <param name="�擾���閼�O ��]:rotation, ���W:position, �傫��:scale"></param>
	/// <returns></returns>
	const DirectX::XMFLOAT3 Layer::GetTransform(const char name[32])
	{
		if (name == "rotation")
		{
			return m_rotation;
		}
		else if (name == "position")
		{
			return m_position;
		}
		else if (name == "scale")
		{
			return m_scale;
		}
		return DirectX::XMFLOAT3(0, 0, 0);
	}

	DirectX::XMFLOAT3 Layer::GetForward()
	{
		return forward;
	}

}