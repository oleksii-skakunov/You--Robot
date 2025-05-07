#pragma once
#include <vector>
#include "Bullet.h"
#include "NPCManager.h"
#include "Team.h"
#include "Player.h"

class BulletManager
{
public:
	BulletManager();
	~BulletManager();
	void Draw() const;
	void Update(float elapsedSec, NPCManager& npcManager, Player& player, HudManager& hudManager, std::vector<std::vector<Vector2f>> levelVerticies);
	void AddBullet(int bulletType, const Vector2f& startPos, const Vector2f& endPos, Team team);
	void DeleteAll();
private:
	std::vector<Bullet*> m_Bullets;
	const Texture m_BulletTexture;
	enum class Type {
		Bullet,
		EnemyBullet
	};
};

