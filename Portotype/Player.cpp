#include "pch.h"
#include "Player.h"
#include "utils.h"
#include <iostream>

Player::Player(float x, float y)
	: m_Bounds{x, y, 20.f, 20.f}
{
}

void Player::Draw() const
{
	utils::SetColor(Color4f(1.f, 0.f, 0.f, 1.f));
	utils::FillRect(m_Bounds);
}

void Player::Move(float elapsedSec)
{
	const float MOVE_SPEED{ 200.f };
	Vector2f direction{0.f, 0.f};

	const Uint8 *pStates = SDL_GetKeyboardState( nullptr );
	if ( pStates[SDL_SCANCODE_RIGHT] )
	{
		++direction.x;
	}
	if ( pStates[SDL_SCANCODE_LEFT])
	{
		--direction.x;
	}
	if (pStates[SDL_SCANCODE_UP])
	{
		++direction.y;
	}
	if (pStates[SDL_SCANCODE_DOWN])
	{
		--direction.y;
	}
	direction = direction.Normalized();

	m_Bounds.left += direction.x * MOVE_SPEED * elapsedSec;
	m_Bounds.bottom += direction.y * MOVE_SPEED * elapsedSec;
}
