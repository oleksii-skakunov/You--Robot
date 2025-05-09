#include "pch.h"
#include "HudHealth.h"

HudHealth::HudHealth(Texture* texture, Rectf pos, int value) : 
	HudObject(texture, pos),
	m_HasUpdatedRecently{ false },
	m_MaxHealth{value},
	m_StartHealth{value},
	m_CurrentHealth{value},
	m_StoredDelta{}
{
}

void HudHealth::Draw() const
{
	if (m_HasUpdatedRecently)
	{
		utils::SetColor(Color4f{ 1.f,1.f,1.f,1.f });
	}
	else {
		utils::SetColor(Color4f{ 1.f,0.f,0.f,1.f });
	}
	for (int i = 0; i < m_CurrentHealth; i++)
	{
		utils::FillRect(m_Pos.left + 1.f + 20 * i, m_Pos.bottom + 1.f, 20.f, 20.f);
	}
	HudObject::Draw();
}

void HudHealth::Update(int delta, float elapsedSec)
{
	if (elapsedSec == 0.f)
	{
		m_StoredDelta = delta;
		m_HasUpdatedRecently = 1;
		m_ElapsedSec = 0;
	}
	else
	{
		m_CurrentHealth += m_StoredDelta;
		m_StoredDelta = 0;
		m_ElapsedSec += elapsedSec;
		if (m_ElapsedSec >= 0.5f)
		{
			m_HasUpdatedRecently = 0;
		}
	}
	

}

int HudHealth::GetMaxHealth()
{
	return m_MaxHealth;
}

void HudHealth::SetMaxHealth(int delta)
{
	m_MaxHealth += delta;
}

void HudHealth::SetCurrentHealth(int delta)
{
	m_CurrentHealth += delta;
}

int HudHealth::GetCurrentHealth()
{
	return m_CurrentHealth;
}

void HudHealth::Restart()
{
	m_MaxHealth = m_StartHealth;
	m_CurrentHealth = m_StartHealth;
}
