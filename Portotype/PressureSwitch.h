#pragma once
class PressureSwitch
{
public:
	PressureSwitch(float left, float bottom, float width, float height);

	void Draw() const;

	Rectf GetBounds() const;
private:
	Rectf m_Bounds;
};

