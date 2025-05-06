#include "pch.h"
#include "HudObject.h"

HudObject::HudObject(Texture* texture, Rectf pos) :
	m_Texture{ texture },
	m_Pos{ pos },
	m_ElapsedSec{}
{
}

HudObject::~HudObject()
{
	delete m_Texture;
	m_Texture = nullptr;
}

void HudObject::Draw() const
{
	m_Texture->Draw(m_Pos);
}
