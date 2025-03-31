#include "pch.h"
#include "Player.h"

Player::Player():
	m_Health{10},
	m_Texture{"robot_3Dblue.png"},
	m_Position{}
{
}

void Player::Draw() const
{
	m_Texture.Draw(Rectf{m_Position.x, m_Position.y, 50.f, 50.f});
}

void Player::Shoot(float angle)
{
}

void Player::Move(Vector2f velocity, float elapsedSec)
{
	m_Position.x += velocity.x * elapsedSec;
	m_Position.y+= velocity.y * elapsedSec;
}
