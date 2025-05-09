#pragma once
#include "HudObject.h"
class HudHealth : public HudObject
{
public:
	HudHealth(Texture* texture, Rectf pos, int value);
	void Draw() const override;
	void Update(int value, float elapsedSec);
	int GetMaxHealth();
	void SetMaxHealth(int delta);
	void SetCurrentHealth(int delta);
	int GetCurrentHealth();
	void Restart();
private:
	int m_MaxHealth;
	int m_StartHealth;
	int m_CurrentHealth;
	bool m_HasUpdatedRecently;
	int m_StoredDelta;
};

