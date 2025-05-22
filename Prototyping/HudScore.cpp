#include "pch.h"
#include "HudScore.h"

HudScore::HudScore(Texture* texture, Rectf pos) :
	HudObject(texture, pos),
	m_Score{},
	m_StoredDelta{}
{
}

void HudScore::Draw() const
{
	HudObject::Draw();
}

void HudScore::Update(int value)
{
	m_Score += value;
	std::string string = std::to_string(m_Score);
	delete HudObject::m_Texture;
	HudObject::m_Texture = new Texture{ string, "Font.ttf", 20, Color4f{1.f,1.f,1.f,1.f} };
}

int HudScore::GetScore()
{
	return m_Score;
}

void HudScore::Restart()
{
	m_Score = 0;
	// Update the texture to show 0
	std::string string = std::to_string(m_Score);
	delete HudObject::m_Texture;
	HudObject::m_Texture = new Texture{ string, "Font.ttf", 20, Color4f{1.f,1.f,1.f,1.f} };
}
