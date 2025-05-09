#include "pch.h"
#include "HudManager.h"

HudManager::HudManager(Rectf window, int health) :
	m_HudHealth{ new HudHealth(new Texture("HudHealth.png"), Rectf(20.f, window.height - 42.f, 202.f, 22.f), health) },
	m_HudScore{ new HudScore(new Texture("", "Font.ttf", 20, Color4f{1.f,1.f,1.f,1.f}), Rectf{window.width - 100.f, window.height - 40.f, 0.f, 0.f}) }
{
}

HudManager::~HudManager()
{
	delete m_HudHealth;
}

void HudManager::Draw() const
{
	m_HudHealth->Draw();
	m_HudScore->Draw();
}

void HudManager::UpdateHealth(float elapsedSec, int delta)
{
	m_HudHealth->Update(delta, elapsedSec);
}

void HudManager::UpdateScore(int delta)
{
	m_HudScore->Update(delta);
}

void HudManager::Restart()
{
	m_HudHealth->Restart();
}
