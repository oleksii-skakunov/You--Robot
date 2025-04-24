#pragma once
#include <vector>
#include "Bullet.h"
#include "NPCManager.h"
class BulletManager
{
public:
	BulletManager();
	~BulletManager();
	void Draw() const;
	void Update(float elapsedSec, NPCManager& npcManager);
	void AddBullet(int bulletType, const Vector2f& startPos, const Vector2f& endPos, int team);
	void DeleteAll();
private:
	std::vector<Bullet*> m_Bullets;
	const Texture m_BulletTexture;
	enum class Type {
		Bullet
	};
};

