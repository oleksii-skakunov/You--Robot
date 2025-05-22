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

void BulletManager::Update(float elapsedSec, NPCManager& npcManager, Player& player, HudManager& hudManager, std::vector<std::vector<Vector2f>> levelVerticies)
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
			m_Bullets[i]->Update(elapsedSec, npcManager, player, hudManager, levelVerticies);
		}
	}
	m_Bullets.erase(std::remove(m_Bullets.begin(), m_Bullets.end(), nullptr), m_Bullets.end());
}

void BulletManager::AddBullet(int bulletType, const Vector2f& startPos, const Vector2f& endPos, Team team)
{
	Type l_bulletType = static_cast<Type>(bulletType);
	float angle{ float(atan2(double((endPos.y - startPos.y)),double((endPos.x - startPos.x)))) };
	Vector2f bulletStartPos{ startPos.x + float(cos(double(angle))) * 55, startPos.y + float(sin(double(angle))) * 55 }; // this makes it so that the bullets don't spawn inside the player
	
	switch (l_bulletType)
	{
	case BulletManager::Type::Bullet:
		m_Bullets.push_back(new Bullet{ 5.f, 4, bulletStartPos, 
			Vector2f{float(cos(double(angle))) * 800, float(sin(double(angle))) * 800}, 
			&m_BulletTexture, angle, team});
		break;
	case BulletManager::Type::EnemyBullet:
		m_Bullets.push_back(new Bullet{ 5.f, 4, bulletStartPos, 
			Vector2f{float(cos(double(angle))) * 400, float(sin(double(angle))) * 400}, 
			&m_BulletTexture, angle, team });
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



