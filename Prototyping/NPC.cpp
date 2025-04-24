#include "pch.h"
#include "NPC.h"

NPC::NPC() :
	m_Health{ 10 },
	m_Bounds{0.f, 0.f, 50.f, 50.f},
	m_CurrentBulletType{ 0 },
	m_Team{Team::Neutral}
{
}

NPC::NPC(Team team, Vector2f pos):
	m_Health{ 10 },
	m_Bounds{ pos.x, pos.y, 50.f, 50.f },
	m_CurrentBulletType{ 0 },
	m_Team{ team }
{
}

void NPC::Draw() const
{
	switch (m_Team)
	{
	case Team::Neutral:
		utils::SetColor(Color4f{ 1.f,1.f,1.f,1.f });
		break;
	case Team::NonTarget:
		utils::SetColor(Color4f{ 0.f,0.f,1.f,1.f });
		break;
	case Team::Target:
		utils::SetColor(Color4f{ 1.f,0.f,0.f,1.f });
		break;
	default:
		break;
	}
	utils::FillRect(m_Bounds);
}

bool NPC::Shoot(Vector2f mousePos)
{
	return false;
}



void NPC::Update(Vector2f velocity, float elapsedSec)
{
	m_Bounds.left += velocity.x * elapsedSec;
	m_Bounds.bottom += velocity.y * elapsedSec;
}

int NPC::GetBulletType()
{
	return m_CurrentBulletType;
}

Vector2f NPC::GetPos()
{
	return Vector2f{ m_Bounds.left , m_Bounds.bottom };
}

Rectf NPC::GetBounds()
{
	return m_Bounds;
}

int NPC::GetHealth()
{
	return m_Health;
}

void NPC::AddHealth(int delta)
{
	m_Health += delta;
}

Team NPC::GetTeam()
{
	return m_Team;
}
