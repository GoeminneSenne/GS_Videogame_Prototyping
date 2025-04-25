#pragma once
class PressureSwitch;
#include <vector>
class Door
{
public:
	Door(PressureSwitch* linkedSwitch, bool isHolding, const Vector2f& start, const Vector2f& end);
	void Update();
	void Draw() const;

	bool isHoldingSwitch() const;
	bool isOpened() const;
	std::vector<Vector2f> GetVertices() const;

	void setIsOpened(bool isOpened);
private:
	PressureSwitch* m_pLinkedSwitch;
	bool m_IsOpened;
	bool m_IsHoldingSwitch;
	std::vector<Vector2f> m_Vertices;
};

