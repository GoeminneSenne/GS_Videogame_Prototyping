#pragma once
#include <vector>

class Player
{
public:
	Player(float x, float y);
	Player(float x, float y, bool isSelected);

	void Draw() const;
	void Update(float elapsedSec, const std::vector<std::vector<Vector2f>>& worldVertexTable);
	void Move(const Vector2f& deltaMovement, const std::vector<std::vector<Vector2f>>& worldVertexTable);

	Rectf GetBounds() const;
	bool IsSelected() const;

	void SetPosition(float x, float y);
	void SetIsSelected(bool isSelected);
private:
	Rectf m_Bounds;
	bool m_IsSelected;
};

