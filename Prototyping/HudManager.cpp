#include "pch.h"
#include "HudManager.h"

HudManager::HudManager(Rectf window, int health) :
	m_HudHealth{ new HudHealth(new Texture("HudHealth.png"), Rectf(20.f, window.height - 42.f, 202.f, 22.f), health) }
{
}

HudManager::~HudManager()
{
	delete m_HudHealth;
}

void HudManager::DrawHealth() const
{
	m_HudHealth->Draw();
}

void HudManager::UpdateHealth(float elapsedSec, int delta)
{
	m_HudHealth->Update(delta, elapsedSec);
}
