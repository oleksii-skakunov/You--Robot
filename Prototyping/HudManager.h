#pragma once
#include "HudObject.h"
#include "HudHealth.h"
#include "HudScore.h"
#include "HudAmmo.h"
#include "iostream"
class HudManager
{
public:
	HudManager(Rectf window, int health, int currentAmmo, int maxAmmo);
	~HudManager();
	void Draw() const;
	void UpdateHealth(float elapsedSec, int delta);
	void UpdateScore(int delta);
	void UpdateAmmo(int currentAmmo);
	int GetScore() { return m_HudScore->GetScore(); }
	void Restart();
private:
	//std::vector<HudObject*> m_HudObjects;
	HudHealth* m_HudHealth;
	HudScore* m_HudScore;
	HudAmmo* m_HudAmmo;
};

