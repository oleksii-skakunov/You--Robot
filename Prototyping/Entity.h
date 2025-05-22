#pragma once
// this class should be a base class for players, npcs and other things that can be placed in the world
// common functions:
// collision handling
// i coild also add here health and shooting stuff and add a toggle for both of them
class Entity
{
public:
	void Draw() const;
	void Update(float elapsedSec);
	void NextFrame();

protected:
	
private:
	bool m_CanShoot;
	bool m_HasHealth;
	bool m_HasAnimation;
};

