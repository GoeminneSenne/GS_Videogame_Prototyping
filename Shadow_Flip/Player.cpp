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
	,m_IsLookingRight{true}
{
}

Player::Player(float x, float y, float width, float height)
	: Player(Vector2f{ x, y }, width, height)
{
}

void Player::Draw() const
{
	utils::FillRect(m_Bounds);
}

void Player::Update(float elapsedSec, const std::vector<std::vector<Vector2f>>& levelVertices)
{
	//Variables
	const float GRAVITY{ -2000.f };
	const float JUMP_POWER{ 600.f };
	const float MOVE_SPEED{ 300.f };
	const float DASH_SPEED{ 600.f };
	const float DOUBLE_JUMP_TRESHOLD{ 0.f };
	const Uint8* pStates = SDL_GetKeyboardState(nullptr);

	m_Velocity.y += GRAVITY * elapsedSec;
	m_Velocity.x = 0.f;

	//Handle input
	if (pStates[SDL_SCANCODE_LEFT])
	{
		m_Velocity.x -= MOVE_SPEED;
	}
	if (pStates[SDL_SCANCODE_RIGHT])
	{
		m_Velocity.x += MOVE_SPEED;
	}
	if (pStates[SDL_SCANCODE_SPACE])
	{
		if (m_IsGrounded)
		{
			m_Velocity.y += JUMP_POWER;
		}
		else if (m_CanDoubleJump and m_IsLight and m_Velocity.y <= DOUBLE_JUMP_TRESHOLD)
		{
			m_Velocity.y = GRAVITY * elapsedSec; //Reset gravity so double jump is consistent

			m_Velocity.y += JUMP_POWER;
			m_CanDoubleJump = false;
		}
	}


	if (m_IsDashing)
	{
		m_Velocity.x = DASH_SPEED;
		if (not m_IsLookingRight) m_Velocity.x *= -1.f;
		m_Velocity.y = 0.f;
		m_DashAccuSec += elapsedSec;

		const float DASH_MAX_SEC{ .3f };
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

	//Move character
	Move(m_Velocity * elapsedSec, levelVertices);
	if (m_IsGrounded or m_HitCeiling)
	{
		m_Velocity.y = 0.f;
	}
}

void Player::Move(const Vector2f& deltaMovement, const std::vector<std::vector<Vector2f>>& levelVertices)
{
	m_Bounds.left += deltaMovement.x;
	m_Bounds.bottom += deltaMovement.y;
	m_IsGrounded = false;
	m_HitCeiling = false;
	utils::HitInfo hitInfo{};

	if (deltaMovement.y != 0.f)
	{
		//Ceiling/Floor collision 
		Vector2f leftRayStart{ m_Bounds.left + 1.f, m_Bounds.bottom - 1.f };
		Vector2f leftRayEnd{ leftRayStart.x, leftRayStart.y + m_Bounds.height };
		Vector2f rightRayStart{ m_Bounds.left + m_Bounds.width, m_Bounds.bottom - 1.f };
		Vector2f rightRayEnd{ leftRayStart.x + m_Bounds.width, rightRayStart.y + m_Bounds.height };

		for (int vectorIdx{ 0 }; vectorIdx < levelVertices.size(); ++vectorIdx)
		{

			if (utils::Raycast(levelVertices[vectorIdx], leftRayStart, leftRayEnd, hitInfo)
				|| utils::Raycast(levelVertices[vectorIdx], rightRayStart, rightRayEnd, hitInfo))
			{
				if (hitInfo.normal.y > 0.f && deltaMovement.y < 0.f)
				{
					m_Bounds.bottom = hitInfo.intersectPoint.y;
					m_IsGrounded = true;
					m_CanDoubleJump = true;
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

	if (deltaMovement.x != 0.f)
	{
		//Wall collision
		Vector2f TopRayStart{ m_Bounds.left - 1.f, m_Bounds.bottom + m_Bounds.height - 1.f };
		Vector2f TopRayEnd{ m_Bounds.left + m_Bounds.width, m_Bounds.bottom + m_Bounds.height - 1.f };
		Vector2f BottomRayStart{ m_Bounds.left - 1.f, m_Bounds.bottom + 1.f };
		Vector2f BottomRayEnd{ m_Bounds.left + m_Bounds.width, m_Bounds.bottom + 1.f };


		for (int vectorIdx{ 0 }; vectorIdx < levelVertices.size(); ++vectorIdx)
		{
			if (utils::Raycast(levelVertices[vectorIdx], BottomRayStart, BottomRayEnd, hitInfo)
				|| utils::Raycast(levelVertices[vectorIdx], TopRayStart, TopRayEnd, hitInfo))
			{
				if (hitInfo.normal.x > 0.f)
				{
					m_Bounds.left = hitInfo.intersectPoint.x + 1.f;
				}
				else if (hitInfo.normal.x < 0.f)
				{
					m_Bounds.left = hitInfo.intersectPoint.x - m_Bounds.width - 1.f;
				}

				break;
			}
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
	m_IsLight = not m_IsLight;
}
