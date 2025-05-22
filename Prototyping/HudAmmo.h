#pragma once
#include "HudObject.h"

class HudAmmo : public HudObject
{
public:
	HudAmmo(Texture* texture, Rectf pos, int currentAmmo, int maxAmmo);
	void Draw() const override;
	void Update(int currentAmmo);
	int GetCurrentAmmo() const;
	int GetMaxAmmo() const;
	void SetCurrentAmmo(int ammo);
	void SetMaxAmmo(int ammo);
	void Restart(int startAmmo);

private:
	int m_CurrentAmmo;
	int m_MaxAmmo;
	int m_StartAmmo;
	Texture* m_TextTexture;
}; 