#include "pch.h"
#include "BulletManager.h"

BulletManager::BulletManager():
	m_Texture{"bullet.png"},
	m_Bullets{}
{
}

void BulletManager::Draw() const
{
	for (int i = 0; i < m_Bullets.size(); i++)
	{
		m_Bullets[i].Draw();
	}
}
