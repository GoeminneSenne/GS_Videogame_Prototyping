#include "pch.h"
#include "Game.h"
#include "Player.h"
#include "utils.h"

Game::Game( const Window& window ) 
	: BaseGame{ window }
	, m_pPlayer1{new Player(200.f, 200.f, true)}
	, m_pPlayer2{new Player(120.f, 400.f, false)}
	, m_Switch{400.f, 120.f, 50.f, 50.f}
	, m_HoldingDoorSwitch{300.f, 370.f, 30.f, 30.f}
	, m_Door{&m_Switch, false, Vector2f(500.f, 100.f), Vector2f(500.f, 350.f)}
	, m_UpperDoor{&m_Switch, true, Vector2f(200.f, 350.f), Vector2f(200.f, 450.f)}
	, m_HoldingDoor{&m_HoldingDoorSwitch, true, Vector2f(550.f, 100.f), Vector2f(550.f, 350.f)}
{
	Initialize();
}

Game::~Game( )
{
	Cleanup( );
}

void Game::Initialize( )
{
	m_WorldVertices.push_back(std::vector<Vector2f>{Vector2f(100.f, 100.f), Vector2f(600.f, 100.f), Vector2f(600.f, 350.f), Vector2f(100.f, 350.f)});
	m_WorldVertices.push_back(std::vector<Vector2f>{Vector2f(100.f, 350.f), Vector2f(600.f, 350.f), Vector2f(600.f, 450.f), Vector2f(100.f, 450.f)});
}

void Game::Cleanup( )
{
	delete m_pPlayer1;
	m_pPlayer1 = nullptr;
}

void Game::Update( float elapsedSec )
{

	m_Switch.CheckCollision(*m_pPlayer1);
	m_Switch.CheckCollision(*m_pPlayer2);
	m_HoldingDoorSwitch.CheckCollision(*m_pPlayer1);
	m_HoldingDoorSwitch.CheckCollision(*m_pPlayer2);
	m_Door.Update();
	m_UpperDoor.Update();
	m_HoldingDoor.Update();

	std::vector<std::vector<Vector2f>> worldVertices{ m_WorldVertices };
	if (!m_Door.isOpened())
	{
		worldVertices.push_back(m_Door.GetVertices());
	}
	if (!m_HoldingDoor.isOpened())
	{
		worldVertices.push_back(m_HoldingDoor.GetVertices());
	}
	if (!m_UpperDoor.isOpened())
	{
		worldVertices.push_back(m_UpperDoor.GetVertices());
	}

	m_pPlayer1->Update(elapsedSec, worldVertices);
	m_pPlayer2->Update(elapsedSec, worldVertices);
	
}

void Game::Draw( ) const
{
	ClearBackground( );
	m_Switch.Draw();
	m_HoldingDoorSwitch.Draw();
	m_Door.Draw();
	m_UpperDoor.Draw();
	m_HoldingDoor.Draw();
	m_pPlayer1->Draw();
	m_pPlayer2->Draw();
	DrawWorldVertices();
}

void Game::ProcessKeyDownEvent( const SDL_KeyboardEvent & e )
{
	//std::cout << "KEYDOWN event: " << e.keysym.sym << std::endl;
	switch (e.keysym.sym)
	{
	case SDLK_f:
		bool isP1Active{ m_pPlayer1->IsSelected() };
		m_pPlayer2->SetIsSelected(isP1Active);
		m_pPlayer1->SetIsSelected(not isP1Active); //Swap boolean of player 1 and 2

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

void Game::DrawWorldVertices() const
{
	utils::SetColor(Color4f(1.f, 0.f, 0.f, 1.f));

	for (int idx{ 0 }; idx < m_WorldVertices.size(); ++idx)
	{
		utils::DrawPolygon(m_WorldVertices[idx], true);
	}
}
