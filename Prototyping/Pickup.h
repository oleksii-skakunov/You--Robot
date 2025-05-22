#pragma once
#include "utils.h"

enum class PickupType
{
	AMMO,
	HEALTH
};

class Pickup
{
public:
	Pickup(PickupType type, const Vector2f& position);
	Rectf GetBounds();
	void Update(float elapsedSec);
	void Draw() const;
	bool IsColliding(const Rectf& playerBounds) const;
	PickupType GetType() const { return m_Type; }
	
private:
	PickupType m_Type;
	Rectf m_Bounds;
	float m_MaxLifetime;
	float m_CurrentLifetime;
};

