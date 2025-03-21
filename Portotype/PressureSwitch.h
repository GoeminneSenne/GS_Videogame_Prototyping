#pragma once
class Player;
class PressureSwitch
{
public:
	PressureSwitch(float left, float bottom, float width, float height);

	void Draw() const;
	void CheckCollision(const Player& player);

	Rectf GetBounds() const;
	bool IsActivated() const;
private:
	Rectf m_Bounds;
	bool m_IsActivated;
};

