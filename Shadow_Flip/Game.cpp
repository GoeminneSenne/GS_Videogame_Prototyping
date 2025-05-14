#include "pch.h"
#include "Game.h"
#include "utils.h"
#include <iostream>

Game::Game( const Window& window ) 
	:BaseGame{ window }
	, m_Player{100.f, 100.f, 30.f, 30.f}
	, m_Camera{GetViewPort().width, GetViewPort().height}
{
	Initialize();
}

Game::~Game( )
{
	Cleanup( );
}

void Game::Initialize( )
{
	std::vector<Vector2f> m_Ground{
		Vector2f{ 0 , 94 },
		Vector2f{ 94 , 94 },
		Vector2f{ 94 , 38 },
		Vector2f{ 188 , 38 },
		Vector2f{ 188 , 65 },
		Vector2f{ 282 , 65 },
		Vector2f{ 282 , 97 },
		Vector2f{ 376 , 97 },
		Vector2f{ 376 , 43 },
		Vector2f{ 470 , 43 },
		Vector2f{ 470 , 114 },
		Vector2f{ 564 , 114 },
		Vector2f{ 564 , 7 },
		Vector2f{ 752 , 7 },
		Vector2f{ 752 , 118 },
		Vector2f{ 846 , 118 },
		Vector2f{ 846 , 25 },
		Vector2f{ 940 , 25 },
		Vector2f{ 846 , 0 },
		Vector2f{ 0 , 0 },
		Vector2f{ 0 , 94 }
	};
	std::vector<Vector2f> m_Platform{
		Vector2f{150, 200},
		Vector2f{150, 250},
		Vector2f{250, 250},
		Vector2f{250, 200},
		Vector2f{150, 200}
	};
	m_Vertices.push_back(m_Ground);
	m_Vertices.push_back(m_Platform);

	CalculateLevelBounds();
}

void Game::Cleanup( )
{
}

void Game::Update( float elapsedSec )
{
	m_Player.Update(elapsedSec, m_Vertices);
}

void Game::Draw( ) const
{
	ClearBackground( );

	m_Camera.Aim(m_LevelBounds.width, m_LevelBounds.height, m_Player.GetPosition());

	m_Player.Draw();

	for (int platformIdx{ 0 }; platformIdx < m_Vertices.size(); ++platformIdx)
	{
		utils::DrawPolygon(m_Vertices[platformIdx]);
	}
	m_Camera.Reset();
}

void Game::ProcessKeyDownEvent( const SDL_KeyboardEvent & e )
{
	//std::cout << "KEYDOWN event: " << e.keysym.sym << std::endl;
}

void Game::ProcessKeyUpEvent( const SDL_KeyboardEvent& e )
{
	//std::cout << "KEYUP event: " << e.keysym.sym << std::endl;
	//switch ( e.keysym.sym )
	//{
	//case SDLK_LEFT:
	//	//std::cout << "Left arrow key released\n";
	//	break;
	//case SDLK_RIGHT:
	//	//std::cout << "`Right arrow key released\n";
	//	break;
	//case SDLK_1:
	//case SDLK_KP_1:
	//	//std::cout << "Key 1 released\n";
	//	break;
	//}
}

void Game::ProcessMouseMotionEvent( const SDL_MouseMotionEvent& e )
{
	//std::cout << "MOUSEMOTION event: " << e.x << ", " << e.y << std::endl;
}

void Game::ProcessMouseDownEvent( const SDL_MouseButtonEvent& e )
{
	//std::cout << "MOUSEBUTTONDOWN event: ";
	//switch ( e.button )
	//{
	//case SDL_BUTTON_LEFT:
	//	std::cout << " left button " << std::endl;
	//	break;
	//case SDL_BUTTON_RIGHT:
	//	std::cout << " right button " << std::endl;
	//	break;
	//case SDL_BUTTON_MIDDLE:
	//	std::cout << " middle button " << std::endl;
	//	break;
	//}
	
}

void Game::ProcessMouseUpEvent( const SDL_MouseButtonEvent& e )
{
	//std::cout << "MOUSEBUTTONUP event: ";
	//switch ( e.button )
	//{
	//case SDL_BUTTON_LEFT:
	//	std::cout << " left button " << std::endl;
	//	break;
	//case SDL_BUTTON_RIGHT:
	//	std::cout << " right button " << std::endl;
	//	break;
	//case SDL_BUTTON_MIDDLE:
	//	std::cout << " middle button " << std::endl;
	//	break;
	//}
}

void Game::ClearBackground( ) const
{
	glClearColor( 0.0f, 0.0f, 0.3f, 1.0f );
	glClear( GL_COLOR_BUFFER_BIT );
}

void Game::CalculateLevelBounds()
{
	Vector2f vertex{ m_Vertices[0][0] };
	m_LevelBounds = Rectf{ vertex.x, vertex.y, vertex.x, vertex.y };

	for (int platformIdx{ 0 }; platformIdx < m_Vertices.size(); ++platformIdx)
	{
		for (int vertexIdx{ 0 }; vertexIdx < m_Vertices[platformIdx].size(); ++vertexIdx)
		{
			Vector2f vertex{ m_Vertices[platformIdx][vertexIdx] };

			if (vertex.x < m_LevelBounds.left) m_LevelBounds.left = vertex.x;
			if (vertex.x > m_LevelBounds.left + m_LevelBounds.width) m_LevelBounds.width = vertex.x - m_LevelBounds.left;
			if (vertex.y < m_LevelBounds.bottom) m_LevelBounds.bottom = vertex.y;
			if (vertex.y > m_LevelBounds.bottom + m_LevelBounds.height) m_LevelBounds.height = vertex.y - m_LevelBounds.bottom;
		}
	}

	std::cout << m_LevelBounds.left << "  " << m_LevelBounds.bottom << "  " << m_LevelBounds.width << "  " << m_LevelBounds.height << "\n";
}