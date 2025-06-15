#include "pch.h"
#include "Camera.h"
#include "utils.h"
#include <iostream>

Camera::Camera(float screenWidth, float screenHeight)
	: m_ScreenWidth{ screenWidth }
	, m_ScreenHeight{ screenHeight }
{
}

Vector2f Camera::Aim(float levelW, float levelH, Vector2f trackCenter) const
{
	Vector2f bottomLeft{ trackCenter.x - m_ScreenWidth / 2.f, trackCenter.y - m_ScreenHeight / 2.f };
	if (bottomLeft.x < 0.f)
	{
		bottomLeft.x = 0.f;
	}
	else if (bottomLeft.x + m_ScreenWidth > levelW)
	{
		bottomLeft.x = levelW - m_ScreenWidth;
	}
	if (bottomLeft.y < 0.f)
	{
		bottomLeft.y = 0.f;
	}
	//else if (bottomLeft.y + m_ScreenHeight > levelH)
	//{
	//	bottomLeft.y = levelH - m_ScreenHeight;
	//}

	glPushMatrix();
	// transform modelview matrix to inverse camera transform!!
	glTranslatef(-bottomLeft.x, -bottomLeft.y, 0.f); //zie slides (viewspace.pdf)

	return bottomLeft;
}

void Camera::Reset() const
{
	glPopMatrix();
}
