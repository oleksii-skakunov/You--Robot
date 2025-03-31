#pragma once
#include <vector>
#include "Bullet.h"
class BulletManager
{
public:
	BulletManager();
	void Draw() const;
	void Update();
	void AddBullet(int bulletType);
private:
	std::vector<Bullet> m_Bullets;
	Texture m_Texture;
	enum class Type {
		Bullet
	};
};

