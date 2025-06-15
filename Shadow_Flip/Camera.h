#pragma once
class Camera
{
public:
	Camera(float screenWidth, float screenHeight);
	Vector2f Aim(float levelW, float levelH, Vector2f trackCenter) const;
	void Reset() const;

private:
	const float m_ScreenWidth;
	const float m_ScreenHeight;
};