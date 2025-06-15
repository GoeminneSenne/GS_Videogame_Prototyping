#pragma once
#include <vector>
class Player
{
public:
	Player(Vector2f pos, float width, float height);
	Player(float x, float y, float width, float height);

	void Draw() const;
	void Update(float elapsedSec, const std::vector<std::vector<Vector2f>>& levelVertices, const std::vector<std::vector<Vector2f>>& specificVertices, const std::vector<Rectf>& shadowAreas);
	void Move(const Vector2f& deltaMovement, const std::vector<std::vector<Vector2f>>& levelVertices, const std::vector<std::vector<Vector2f>>& specificVertices);
	void Dash();

	Vector2f GetPosition() const;
	const Rectf& GetBounds() const;
	void SetPosition(const Vector2f& pos);
	bool IsLight() const;
	bool IsUsingLens() const;
	void ShadowFlip();
	float GetLensTime() const;
	float GetLensMax() const;
	void ResetLensTime();
	int GetLives() const;

	void Respawn(const Vector2f& pos);
	
private:
	Rectf m_Bounds;
	const float m_Width;
	const float m_Height;
	Vector2f m_Velocity;
	bool m_IsGrounded;
	bool m_HitCeiling;
	
	bool m_IsLight;
	bool m_IsLookingRight;
	bool m_CanDoubleJump;

	float m_GlideTimer;
	
	bool m_IsDashing;
	float m_DashAccuSec;
	
	bool m_IsInShadowArea;
	Rectf m_CurrentShadowBounds;

	bool m_IsUsingLens;
	const float m_LensTimerMax;
	float m_LensTimer;
	
	int m_Lives;

	void CheckWallCollision(const Vector2f& deltaMovement, const std::vector<std::vector<Vector2f>>& vertices);
	void CheckVerticalCollision(const Vector2f& deltaMovement, const std::vector<std::vector<Vector2f>>& vertices);
	bool IsShadowUnderWall(const std::vector<std::vector<Vector2f>>& vertices);
};

