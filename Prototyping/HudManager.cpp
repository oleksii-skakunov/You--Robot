#include "pch.h"
#include "HudManager.h"

HudManager::HudManager(Rectf window, int health, int currentAmmo, int maxAmmo) :
	m_HudHealth{ new HudHealth(new Texture("HudHealth.png"), Rectf(20.f, window.height - 42.f, 202.f, 22.f), health) },
	m_HudScore{ new HudScore(new Texture("", "Font.ttf", 20, Color4f{1.f,1.f,1.f,1.f}), Rectf{window.width - 100.f, window.height - 40.f, 0.f, 0.f}) },
	m_HudAmmo{ new HudAmmo(new Texture("HudAmmo.png"), Rectf(20.f, window.height - 70.f, 202.f, 22.f), currentAmmo, maxAmmo) }
{
}

HudManager::~HudManager()
{
	delete m_HudHealth;
	delete m_HudScore;
	delete m_HudAmmo;
}

void HudManager::Draw() const
{
	m_HudHealth->Draw();
	m_HudScore->Draw();
	m_HudAmmo->Draw();
}

void HudManager::UpdateHealth(float elapsedSec, int delta)
{
	m_HudHealth->Update(delta, elapsedSec);
}

void HudManager::UpdateScore(int delta)
{
	m_HudScore->Update(delta);
}

void HudManager::UpdateAmmo(int currentAmmo)
{
	m_HudAmmo->Update(currentAmmo);
}

void HudManager::Restart()
{
	m_HudHealth->Restart();
	m_HudAmmo->Restart(50); // Reset to starting ammo value (50)
	m_HudScore->Restart(); // Reset score to 0
}
