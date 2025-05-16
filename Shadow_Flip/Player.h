#pragma once
#include <vector>
class Player
{
public:
	Player(Vector2f pos, float width, float height);
	Player(float x, float y, float width, float height);

	void Draw() const;
	void Update(float elapsedSec, const std::vector<std::vector<Vector2f>>& levelVertices);
	void Move(const Vector2f& deltaMovement, const std::vector<std::vector<Vector2f>>& levelVertices);

	Vector2f GetPosition() const;
	void SetPosition(const Vector2f& pos);
	bool IsLight() const;
	void ShadowFlip();
private:
	Rectf m_Bounds;
	Vector2f m_Velocity;
	bool m_IsGrounded;
	bool m_HitCeiling;
	bool m_HasDoubleJump;
	bool m_IsLight;
};

