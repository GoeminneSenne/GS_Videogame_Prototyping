#include "pch.h"
#include "Player.h"
#include "utils.h"
#include <iostream>

Player::Player(Vector2f pos, float width, float height)
	: m_Bounds{pos.x, pos.y, width, height}
	, m_Velocity{0.f, 0.f}
	, m_IsGrounded{false}
	, m_HitCeiling{false}
	, m_CanDoubleJump{true}
	, m_IsLight{true}
	, m_IsDashing{false}
	, m_IsLookingRight{true}
	, m_DashAccuSec{0.f}
	, m_Width{width}
	, m_Height{height}
	, m_IsInShadowArea{false}
	, m_CurrentShadowBounds{}
	, m_GlideTimer{0.f}
{
}

Player::Player(float x, float y, float width, float height)
	: Player(Vector2f{ x, y }, width, height)
{
}

void Player::Draw() const
{
	const Color4f lightColor{ 0.9f, 1.f, 0.6f, 1.f };
	const Color4f shadowColor{ 0.33f, 0.f, 0.66f, 1.f };

	if (m_IsLight) utils::SetColor(lightColor);
	else utils::SetColor(shadowColor);

	utils::FillRect(m_Bounds);
}

void Player::Update(float elapsedSec, const std::vector<std::vector<Vector2f>>& levelVertices, const Rectf& shadowArea)
{
	//Variables
	const float GRAVITY{ -2000.f };
	const float MOVE_SPEED{ 300.f };
	const float DASH_SPEED{ 650.f };
	const float JUMP_POWER{ 600.f };
	const float DOUBLE_JUMP_TRESHOLD{ 5.f };
	const float SUPER_JUMP_POWER{1000.f};

	const Uint8* pStates = SDL_GetKeyboardState(nullptr);

	m_Velocity.x = 0.f;
	m_Velocity.y += GRAVITY * elapsedSec;


	//Handle input
	if (pStates[SDL_SCANCODE_LEFT])
	{
		m_Velocity.x -= MOVE_SPEED;
	}
	if (pStates[SDL_SCANCODE_RIGHT])
	{
		m_Velocity.x += MOVE_SPEED;
	}
	if (pStates[SDL_SCANCODE_SPACE] && not m_IsInShadowArea)
	{
		if (m_IsGrounded)
		{
			if (pStates[SDL_SCANCODE_UP])
			{
				m_Velocity.y += SUPER_JUMP_POWER;
				m_CanDoubleJump = false;
			}
			else
			{
				m_Velocity.y += JUMP_POWER;
				m_CanDoubleJump = true;
			}
		}
		else if (m_CanDoubleJump and m_IsLight and m_Velocity.y <= DOUBLE_JUMP_TRESHOLD)
		{
			m_Velocity.y = GRAVITY * elapsedSec; //Reset gravity so double jump is consistent

			m_Velocity.y += JUMP_POWER * 0.75f; //2e jump is minder hoog
			m_CanDoubleJump = false;
		}
		else if (m_Velocity.y <= 0.f)
		{
			const float GLIDE_MAX_TIME{ 0.7f };
			if (m_GlideTimer <= GLIDE_MAX_TIME)
			{
				m_GlideTimer += elapsedSec;

				m_Velocity.y = GRAVITY * elapsedSec; //Gliding has constant downward speed instead of accelerating
				m_Velocity.x /= 2.3f; // Halve movespeed while gliding
			}

		}
	}


	if (m_IsDashing)
	{
		m_Velocity.x = DASH_SPEED;
		if (not m_IsLookingRight) m_Velocity.x *= -1.f;
		if(m_Velocity.y < 0.f) m_Velocity.y = 0.f;
		m_DashAccuSec += elapsedSec;

		const float DASH_MAX_SEC{ .5f };
		if (m_DashAccuSec >= DASH_MAX_SEC)
		{
			m_IsDashing = false;
		}
	}
	else
	{
		if (m_Velocity.x > 0.f)
		{
			m_IsLookingRight = true;
		}
		else if (m_Velocity.x < 0.f)
		{
			m_IsLookingRight = false;
		}
	}

	//Adjust velocity in certain cases:
	if (m_IsInShadowArea) m_Velocity.y = 0.f;
	if (m_Velocity.y > JUMP_POWER * 0.75f && not m_CanDoubleJump) m_Velocity.x /= 2.2f;

	//Move character
	Move(m_Velocity * elapsedSec, levelVertices);
	if (m_IsGrounded or m_HitCeiling)
	{
		m_Velocity.y = 0.f;
	
		m_GlideTimer = 0.f;
	}


	if (utils::IsOverlapping(shadowArea, m_Bounds) && pStates[SDL_SCANCODE_DOWN] && m_IsGrounded && not m_IsInShadowArea && not m_IsLight)
	{
		std::cout << "Entered shadow\n";
		m_IsInShadowArea = true;
		m_CurrentShadowBounds = shadowArea;

		m_Bounds.bottom = shadowArea.bottom;
		m_Bounds.height = shadowArea.height;
	}
	else if (m_IsInShadowArea && pStates[SDL_SCANCODE_UP])
	{

		m_Bounds.height = m_Height;
		if (not IsShadowUnderWall(levelVertices))
		{
			std::cout << "Exited shadow\n";
			m_IsInShadowArea = false;
			m_CurrentShadowBounds = Rectf();
		}
		else
		{
			m_Bounds.height = shadowArea.height;
		}


	}

}

void Player::Move(const Vector2f& deltaMovement, const std::vector<std::vector<Vector2f>>& levelVertices)
{
	m_Bounds.left += deltaMovement.x;
	m_Bounds.bottom += deltaMovement.y;
	
	if (not m_IsInShadowArea)
	{
		if(deltaMovement.y != 0.f)  CheckVerticalCollision(deltaMovement, levelVertices);
		if(deltaMovement.x != 0.f)	CheckWallCollision(deltaMovement, levelVertices);
	}
	else
	{
		if (m_Bounds.left < m_CurrentShadowBounds.left)
		{
			m_Bounds.left = m_CurrentShadowBounds.left;
		}
		if(m_Bounds.left + m_Bounds.width > m_CurrentShadowBounds.left + m_CurrentShadowBounds.width)
		{
			m_Bounds.left = m_CurrentShadowBounds.left + m_CurrentShadowBounds.width - m_Bounds.width;
		}
	}
}

void Player::Dash()
{
	if (m_IsDashing or m_IsLight) return;

	m_IsDashing = true;
	m_DashAccuSec = 0.f;
}

Vector2f Player::GetPosition() const
{
	return Vector2f(m_Bounds.left, m_Bounds.bottom);
}

void Player::SetPosition(const Vector2f& pos)
{
	m_Bounds.left = pos.x;
	m_Bounds.bottom = pos.y;
}

bool Player::IsLight() const
{
	return m_IsLight;
}

void Player::ShadowFlip()
{
	if (m_IsInShadowArea) return;

	m_IsLight = not m_IsLight;
}

void Player::CheckWallCollision(const Vector2f& deltaMovement, const std::vector<std::vector<Vector2f>>& vertices)
{
	//if (deltaMovement.x == 0.f) return;

	utils::HitInfo hitInfo;
	Vector2f TopRayStart{ m_Bounds.left - 1.f, m_Bounds.bottom + m_Bounds.height - 1.f };
	Vector2f TopRayEnd{ m_Bounds.left + m_Bounds.width, m_Bounds.bottom + m_Bounds.height - 1.f };
	Vector2f BottomRayStart{ m_Bounds.left - 1.f, m_Bounds.bottom + 1.f };
	Vector2f BottomRayEnd{ m_Bounds.left + m_Bounds.width, m_Bounds.bottom + 1.f };


	for (int vectorIdx{ 0 }; vectorIdx < vertices.size(); ++vectorIdx)
	{
		if (utils::Raycast(vertices[vectorIdx], BottomRayStart, BottomRayEnd, hitInfo)
			|| utils::Raycast(vertices[vectorIdx], TopRayStart, TopRayEnd, hitInfo))
		{
			if (hitInfo.normal.x > 0.f)
			{
				m_Bounds.left = hitInfo.intersectPoint.x + 1.f;
			}
			else if (hitInfo.normal.x < 0.f)
			{
				m_Bounds.left = hitInfo.intersectPoint.x - m_Bounds.width - 1.f;
			}
		}
	}
}
void Player::CheckVerticalCollision(const Vector2f& deltaMovement, const std::vector<std::vector<Vector2f>>& vertices)
{
	//if (deltaMovement.y != 0.f)
	{
		m_IsGrounded = false;
		m_HitCeiling = false;

		utils::HitInfo hitInfo;
		Vector2f leftRayStart{ m_Bounds.left + 1.f, m_Bounds.bottom - 1.f };
		Vector2f leftRayEnd{ leftRayStart.x, leftRayStart.y + m_Bounds.height };
		Vector2f rightRayStart{ m_Bounds.left + m_Bounds.width, m_Bounds.bottom - 1.f };
		Vector2f rightRayEnd{ leftRayStart.x + m_Bounds.width, rightRayStart.y + m_Bounds.height };

		for (int vectorIdx{ 0 }; vectorIdx < vertices.size(); ++vectorIdx)
		{

			if (utils::Raycast(vertices[vectorIdx], leftRayStart, leftRayEnd, hitInfo)
				|| utils::Raycast(vertices[vectorIdx], rightRayStart, rightRayEnd, hitInfo))
			{
				if (hitInfo.normal.y > 0.f && deltaMovement.y < 0.f)
				{
					m_Bounds.bottom = hitInfo.intersectPoint.y;
					m_IsGrounded = true;
				}
				else if (hitInfo.normal.y < 0.f)
				{
					m_Bounds.bottom = hitInfo.intersectPoint.y - m_Bounds.height - 1.f;
					m_HitCeiling = true;
				}
				break;
			}
		}
	}
}

bool Player::IsShadowUnderWall(const std::vector<std::vector<Vector2f>>& vertices)
{
	utils::HitInfo hitInfo;
	Vector2f TopRayStart{ m_Bounds.left - 1.f, m_Bounds.bottom + m_Bounds.height - 1.f };
	Vector2f TopRayEnd{ m_Bounds.left + m_Bounds.width, m_Bounds.bottom + m_Bounds.height - 1.f };
	Vector2f BottomRayStart{ m_Bounds.left - 1.f, m_Bounds.bottom + 1.f };
	Vector2f BottomRayEnd{ m_Bounds.left + m_Bounds.width, m_Bounds.bottom + 1.f };


	for (int vectorIdx{ 0 }; vectorIdx < vertices.size(); ++vectorIdx)
	{
		if (utils::Raycast(vertices[vectorIdx], BottomRayStart, BottomRayEnd, hitInfo)
			|| utils::Raycast(vertices[vectorIdx], TopRayStart, TopRayEnd, hitInfo))
		{
			return true;
		}
	}
	return false;
}

