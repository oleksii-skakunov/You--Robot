#pragma once
#include "HudObject.h"
#include "HudHealth.h"
#include "iostream"
class HudManager
{
public:
	HudManager(Rectf window, int health);
	~HudManager();
	void DrawHealth() const;
	void UpdateHealth(float elapsedSec, int delta);
private:
	//std::vector<HudObject*> m_HudObjects;
	HudHealth* m_HudHealth;
};

