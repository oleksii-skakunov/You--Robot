#pragma once
#include "HudObject.h"
#include "HudHealth.h"
#include "HudScore.h"
#include "iostream"
class HudManager
{
public:
	HudManager(Rectf window, int health);
	~HudManager();
	void Draw() const;
	void UpdateHealth(float elapsedSec, int delta);
	void UpdateScore(int delta);
	void Restart();
private:
	//std::vector<HudObject*> m_HudObjects;
	HudHealth* m_HudHealth;
	HudScore* m_HudScore;
};

