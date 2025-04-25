#include "pch.h"
#include "Game.h"
#include "Player.h"
#include "utils.h"
#include "Level.h"

Game::Game( const Window& window ) 
	: BaseGame{ window }
	, m_pPlayer1{new Player(200.f, 200.f, true)}
	, m_pPlayer2{new Player(120.f, 400.f, false)}
	, m_LoadingAccuSec{0.f}
	, m_pLevel{new Level{}}
{
	Initialize();
}

Game::~Game( )
{
	Cleanup( );
}

void Game::Initialize( )
{
}

void Game::Cleanup( )
{
	delete m_pPlayer1;
	delete m_pPlayer2;
	delete m_pLevel;
}

void Game::Update( float elapsedSec )
{
	if (utils::IsOverlapping(m_pPlayer1->GetBounds(), m_pLevel->GetEndCollectible()) || utils::IsOverlapping(m_pPlayer2->GetBounds(), m_pLevel->GetEndCollectible()))
	{
		const float loadingTime{ 2.5f };

		m_LoadingAccuSec += elapsedSec;
		if (m_LoadingAccuSec >= loadingTime)
		{
			m_LoadingAccuSec = 0.f;
			ResetLevel();
		}

		return;
	}

	m_pLevel->Update(*m_pPlayer1, *m_pPlayer2);

	std::vector<std::vector<Vector2f>> levelVertices{ levelVertices };
	for (const Door& door : m_pLevel->GetDoors())
	{
		if (!door.isOpened())
		{
			levelVertices.push_back(door.GetVertices());
		}
	}

	m_pPlayer1->Update(elapsedSec, levelVertices);
	m_pPlayer2->Update(elapsedSec, levelVertices);
	
}

void Game::Draw( ) const
{
	ClearBackground( );

	if (utils::IsOverlapping(m_pPlayer1->GetBounds(), m_pLevel->GetEndCollectible()) || utils::IsOverlapping(m_pPlayer2->GetBounds(), m_pLevel->GetEndCollectible()))
	{
		utils::SetColor(Color4f(0.f, 0.f, 0.f, 1.f));
		utils::FillRect(GetViewPort());
		return;
	}

	m_pLevel->Draw();
	m_pPlayer1->Draw();
	m_pPlayer2->Draw();
}

void Game::ProcessKeyDownEvent( const SDL_KeyboardEvent & e )
{
	//std::cout << "KEYDOWN event: " << e.keysym.sym << std::endl;
	switch (e.keysym.sym)
	{
	case SDLK_f:
		m_pPlayer2->SetIsSelected(m_pPlayer1->IsSelected());
		m_pPlayer1->SetIsSelected(not m_pPlayer1->IsSelected()); //Swap boolean of player 1 and 2

		break;
	default:
		break;
	}
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

void Game::ResetLevel()
{
	m_pPlayer1->SetPosition(200.f, 200.f);
	m_pPlayer1->SetIsSelected(true);
	m_pPlayer2->SetPosition(120.f, 400.f);
	m_pPlayer2->SetIsSelected(false);
	
	m_pLevel = new Level{};
}
