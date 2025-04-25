#include "pch.h"
#include "Player.h"
#include "utils.h"
#include <iostream>

Player::Player(float x, float y)
	: Player{ x, y, true }
{
}

Player::Player(float x, float y, bool isSelected)
	: m_Bounds{x,y,20.f,20.f}
	, m_IsSelected{isSelected}
{
}

void Player::Draw() const
{
	utils::SetColor(Color4f(0.6f , 0.6f, 1.f, 1.f));
	utils::FillRect(m_Bounds);

	if (m_IsSelected)
	{
		utils::SetColor(Color4f{ 1.f, 0.7f, 0.f, 1.f });
		utils::DrawRect(m_Bounds, 3.f);
	}
}

void Player::Update(float elapsedSec, const std::vector<std::vector<Vector2f>>& worldVertexTable)
{
	if (not m_IsSelected) return;

	const float MOVE_SPEED{ 200.f };
	Vector2f direction{0.f, 0.f};

	const Uint8 *pStates = SDL_GetKeyboardState( nullptr );
	if ( pStates[SDL_SCANCODE_RIGHT] or pStates[SDL_SCANCODE_D])
	{
		++direction.x;
	}
	if ( pStates[SDL_SCANCODE_LEFT] or pStates[SDL_SCANCODE_A])
	{
		--direction.x;
	}
	if (pStates[SDL_SCANCODE_UP] or pStates[SDL_SCANCODE_W])
	{
		++direction.y;
	}
	if (pStates[SDL_SCANCODE_DOWN] or pStates[SDL_SCANCODE_S])
	{
		--direction.y;
	}
	direction = direction.Normalized();

	Vector2f deltaMovement{ direction * MOVE_SPEED * elapsedSec };
	this->Move(deltaMovement, worldVertexTable);
}

void Player::Move(const Vector2f& deltaMovement, const std::vector<std::vector<Vector2f>>& worldVertexTable)
{

	m_Bounds.left += deltaMovement.x;
	m_Bounds.bottom += deltaMovement.y;

	//m_IsGrounded = false;

	Vector2f leftRayStart{ m_Bounds.left, m_Bounds.bottom };
	Vector2f leftRayEnd{ leftRayStart.x, leftRayStart.y + m_Bounds.height };
	Vector2f rightRayStart{ m_Bounds.left + m_Bounds.width, m_Bounds.bottom };
	Vector2f rightRayEnd{ leftRayStart.x + m_Bounds.width, rightRayStart.y + m_Bounds.height };

	utils::HitInfo hitInfo{};
	for (int vectorIdx{0}; vectorIdx < worldVertexTable.size(); ++vectorIdx)
	{
		if (utils::Raycast(worldVertexTable[vectorIdx], leftRayStart, leftRayEnd, hitInfo)
			|| utils::Raycast(worldVertexTable[vectorIdx], rightRayStart, rightRayEnd, hitInfo))
		{
			if (deltaMovement.y < 0.f)
			{
				m_Bounds.bottom = hitInfo.intersectPoint.y;
			}
			else if (deltaMovement.y > 0.f)
			{
				m_Bounds.bottom = hitInfo.intersectPoint.y - m_Bounds.height;
			}

		}

		if (utils::Raycast(worldVertexTable[vectorIdx], leftRayStart, rightRayStart, hitInfo)
			|| utils::Raycast(worldVertexTable[vectorIdx], leftRayEnd, rightRayEnd, hitInfo))
		{
			if (deltaMovement.x < 0.f)
			{
				m_Bounds.left = hitInfo.intersectPoint.x;
			}
			else if (deltaMovement.x > 0.f)
			{
				m_Bounds.left = hitInfo.intersectPoint.x - m_Bounds.width;
			}
		}

	}

}

Rectf Player::GetBounds() const
{
	return m_Bounds;
}

bool Player::IsSelected() const
{
	return m_IsSelected;
}

void Player::SetPosition(float x, float y)
{
	m_Bounds.left = x;
	m_Bounds.bottom = y;
}

void Player::SetIsSelected(bool isSelected)
{
	m_IsSelected = isSelected;
}
