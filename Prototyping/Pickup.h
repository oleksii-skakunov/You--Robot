#pragma once
#include "PickupType.h"
class Pickup
{
public:
	Pickup(Vector2f pos, Type type );
	Rectf GetBounds();
	void Update(float elapsedSec);
private:
	
	Rectf m_Bounds;
	float m_MaxLifetime;
	float m_CurrentLifetime;
};

