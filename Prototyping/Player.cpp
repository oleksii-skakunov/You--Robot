#include "pch.h"
#include "Player.h"

Player::Player():
	m_Health{10},
	m_Texture{"robot_3Dblue.png"},
	m_Position{},
	m_CurrentBulletType{0}
{
}

void Player::Draw() const
{
	m_Texture.Draw(Rectf{m_Position.x, m_Position.y, 50.f, 50.f});
}

bool Player::Shoot(Vector2f mousePos)
{
	float angle{ float(atan(double((mousePos.x - m_Position.x) / (mousePos.y - m_Position.y)))) };
	// check if has enough ammo and follows the laws
	return true;
}



void Player::Move(Vector2f velocity, float elapsedSec)
{
	m_Position.x += velocity.x * elapsedSec;
	m_Position.y+= velocity.y * elapsedSec;
}

int Player::GetBulletType()
{
	return m_CurrentBulletType;
}

Vector2f Player::GetPos()
{
	return m_Position;
}
