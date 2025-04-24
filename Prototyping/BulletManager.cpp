#include "pch.h"
#include "BulletManager.h"

BulletManager::BulletManager():
	m_BulletTexture{"bullet1.png"},
	m_Bullets{}
{
}

BulletManager::~BulletManager()
{
	DeleteAll();
}


void BulletManager::Draw() const
{
	for (int i = 0; i < m_Bullets.size(); i++)
	{
		m_Bullets[i]->Draw();
	}
}

void BulletManager::Update(float elapsedSec, NPCManager& npcManager)
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
			m_Bullets[i]->Update(elapsedSec, npcManager);
		}
	}
	m_Bullets.erase(std::remove(m_Bullets.begin(), m_Bullets.end(), nullptr), m_Bullets.end());
}

void BulletManager::AddBullet(int bulletType, const Vector2f& startPos, const Vector2f& endPos, int team)
{
	Type l_bulletType = static_cast<Type>(bulletType);
	float angle{ float(atan2(double((endPos.y - startPos.y)),double((endPos.x - startPos.x)))) };
	switch (l_bulletType)
	{
	case BulletManager::Type::Bullet:
		
		
		m_Bullets.push_back(new Bullet{ 5.f, 1, startPos, Vector2f{float(cos(double(angle)))*1000.f,float(sin(double(angle))) * 1000.f}, &m_BulletTexture, angle, team});
		break;
	default:
		break;
	}
}

void BulletManager::DeleteAll()
{
	for (int i = 0; i < m_Bullets.size(); i++)
	{
		delete m_Bullets[i];
		m_Bullets[i] = nullptr;
	}
	m_Bullets.clear();
}



