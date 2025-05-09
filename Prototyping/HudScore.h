#pragma once
#include "HudObject.h"
class HudScore : public HudObject
{
public:
	HudScore(Texture* texture, Rectf pos);
	void Draw() const override;
	void Update(int value);
	int GetScore();
	void Restart();
private:
	int m_Score;
	int m_StoredDelta;
};

