#include "pch.h"
#include "Bullet.h"



Bullet::Bullet(float traveltime, int damage, const Vector2f startPos, Vector2f velocityVector, const Texture* pTexture, float angle, Team team):
	m_Lifetime{ 0 },
	m_Traveltime{ traveltime },
	m_Damage{ damage },
	m_Pos{ startPos },
	m_VelocityVector{ velocityVector },
	m_pTexture{ pTexture },
	m_Angle{ angle * 57.29f },
	// also rewrite the update function so that we don't recalculate the velocityVector every frame
	m_HasStopped{ 0 },
	m_Team{static_cast<Team>(team)}
{ 
}

Bullet::~Bullet()
{
}

void Bullet::Draw() const
{
	glPushMatrix();
	glTranslatef(m_Pos.x, m_Pos.y, 0);
	glRotatef(m_Angle + 90, 0, 0, 1); // COUNTERCLOCKWISE!!!
	m_pTexture->Draw();
	glPopMatrix();
}

void Bullet::Update(float elapsedSec, NPCManager& npcManager)
{
	Rectf l_BulletHitbox{ m_Pos.x, m_Pos.y, 1.f, 1.f };
	for (int i = 0; i < npcManager.GetNPCVector().size(); i++)
	{
		if (l_BulletHitbox.left < npcManager.GetNPCVector()[i]->GetBounds().left + npcManager.GetNPCVector()[i]->GetBounds().width &&
			l_BulletHitbox.left + l_BulletHitbox.width > npcManager.GetNPCVector()[i]->GetBounds().left &&
			l_BulletHitbox.bottom < npcManager.GetNPCVector()[i]->GetBounds().bottom + npcManager.GetNPCVector()[i]->GetBounds().height &&
			l_BulletHitbox.bottom + l_BulletHitbox.height > npcManager.GetNPCVector()[i]->GetBounds().bottom)
		{
			npcManager.GetNPCVector()[i]->AddHealth(m_Damage * -1);
			m_HasStopped = 1;
		}

	}
	if (m_Lifetime >= m_Traveltime)
	{
		m_HasStopped = 1;
	}
	else {
		m_Lifetime += elapsedSec;
		m_Pos.x += m_VelocityVector.x * elapsedSec;
		m_Pos.y += m_VelocityVector.y * elapsedSec;
	}
	
}

bool Bullet::HasStopped() const
{
	return m_HasStopped;
}
