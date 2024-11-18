#include "Thruster.h"

namespace TMF
{
	Thruster::Thruster()
	{
	}
	Thruster::~Thruster()
	{
	}
	void Thruster::OnInitialize()
	{
	}
	void Thruster::OnFinalize()
	{
	}
	void Thruster::OnUpdate()
	{
	}
	void Thruster::OnLateUpdate()
	{
	}
	void Thruster::OnDraw()
	{
	}
	void Thruster::OnDrawImGui()
	{
	}
	void Thruster::StartThruster()
	{
		if (m_thrusterValue > 0)
		{
			m_isThruster = true;
			// ‰ŠúÁ”ï
			m_thrusterValue += m_maxThrusterValue * m_firstUseMagnification;
			if (m_thrusterValue >= m_maxThrusterValue)
			{
				m_isOverHeat = true;
				m_thrusterValue = m_maxThrusterValue;
			}
		}
		
	}
}