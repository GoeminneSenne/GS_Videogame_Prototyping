#pragma once
class Player
{
public:
	Player(float x, float y);

	void Draw() const;
	void Move(float elapsedSec);

private:
	Rectf m_Bounds;
};

