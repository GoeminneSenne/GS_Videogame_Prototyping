#pragma once
class Player;
#include "Door.h"
class PressureSwitch
{
public:
	PressureSwitch(float left, float bottom, float width, float height);

	void Draw() const;
	void CheckCollision(const Player& player1, const Player& player2);

	Rectf GetBounds() const;
	bool IsActivated() const;
private:
	Rectf m_Bounds;
	bool m_IsActivated;
};

