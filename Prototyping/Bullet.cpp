#include "pch.h"
#include "Bullet.h"



Bullet::Bullet(float traveltime, int damage, Vector2f startPos, float velocity, Texture* pTexture, float angle, int team):
	m_Traveltime{ traveltime },
	m_Damage{ damage },
	m_Pos{ startPos },
	m_Velocity{ velocity },
	m_pTexture{ pTexture },
	m_Angle{ angle },
	m_HasStopped{ 0 },
	m_Team{static_cast<Team>(team)}
{
}

void Bullet::Draw() const
{
	glPushMatrix();
	glTranslatef(-m_Pos.x, -m_Pos.y, 0);
	glRotatef(m_Angle + 90, 0, 0, 1); // COUNTERCLOCKWISE!!!
	m_pTexture->Draw();
	glPopMatrix();
}

void Bullet::Update(float elapsedSec)
{
	if (m_Lifetime >= m_Traveltime)
	{
		m_HasStopped = 1;
	}
	else {
		m_Lifetime += elapsedSec;
		Vector2f velocityVector{ m_Velocity / cos(double(m_Angle)), tan(double(m_Angle)) * m_Velocity };
		m_Pos.x += velocityVector.x * elapsedSec;
		m_Pos.y += velocityVector.y * elapsedSec;
	}
}
