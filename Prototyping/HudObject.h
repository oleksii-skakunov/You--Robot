#pragma once
#include "Texture.h"
#include "utils.h"
class HudObject
{
public:
	HudObject(Texture* texture, Rectf pos);
	~HudObject();
	virtual void Draw() const;
	//virtual void Update(int value, float elapsedSec) =0;
protected:
	Texture* m_Texture;
	Rectf m_Pos;
	float m_ElapsedSec;
};

