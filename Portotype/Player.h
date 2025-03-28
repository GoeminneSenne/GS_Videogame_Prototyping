#pragma once
#include <vector>

class Player
{
public:
	Player(float x, float y);

	void Draw() const;
	void Update(float elapsedSec, const std::vector<std::vector<Vector2f>>& worldVertexTable);
	void Move(const Vector2f& deltaMovement, const std::vector<std::vector<Vector2f>>& worldVertexTable);

	Rectf GetBounds() const;
private:
	Rectf m_Bounds;
};

