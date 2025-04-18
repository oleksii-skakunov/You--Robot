#include "pch.h"
#include "BulletManager.h"

BulletManager::BulletManager():
	m_BulletTexture{"bullet1.png"},
	m_Bullets{}
{
}

void BulletManager::Draw() const
{
	for (int i = 0; i < m_Bullets.size(); i++)
	{
		m_Bullets[i]->Draw();
	}
}

void BulletManager::Update(float elapsedSec)
{
	for (int i = 0; i < m_Bullets.size(); i++)
	{
		if (m_Bullets[i]->HasStopped())
		{
			delete m_Bullets[i];
			m_Bullets[i] = nullptr;
		}
		else
		{
			m_Bullets[i]->Update(elapsedSec);
		}
	}
	m_Bullets.erase(std::remove(m_Bullets.begin(), m_Bullets.end(), nullptr), m_Bullets.end());
}

void BulletManager::AddBullet(int bulletType, const Vector2f& startPos, const Vector2f& endPos, int team)
{
	Type l_bulletType = static_cast<Type>(bulletType);
	float angle{ float(atan(double((endPos.x - startPos.x) / (endPos.y - startPos.y)))) * 57.29f };
	switch (l_bulletType)
	{
	case BulletManager::Type::Bullet:
		m_Bullets.push_back(new Bullet{ 5.f, 1, startPos, 200.f, &m_BulletTexture, angle, team });
		break;
	default:
		break;
	}
}

void BulletManager::DeleteAll()
{
	m_Bullets.clear();
}


