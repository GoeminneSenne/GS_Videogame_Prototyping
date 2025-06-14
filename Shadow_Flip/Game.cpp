#include "pch.h"
#include "Game.h"
#include "utils.h"
#include <iostream>
#include "SVGParser.h"

Game::Game( const Window& window ) 
	:BaseGame{ window }
	, m_Player{100.f, 100.f, 40.f, 40.f}
	, m_Camera{GetViewPort().width, GetViewPort().height}
	, m_StartingPosition{100.f, 100.f}
	, m_LevelBounds{0,0,0,0}
	, m_ShadowArea{}
{
	Initialize();
}

Game::~Game( )
{
	Cleanup( );
}

void Game::Initialize( )
{


	//m_ShadowArea = Rectf(100, 35, 150, 5);
	CreateLevel();
}

void Game::Cleanup( )
{
}

void Game::Update( float elapsedSec )
{
	std::vector<std::vector<Vector2f>> activeVertices{m_SharedVertices};
	if (m_Player.IsLight())
	{
		activeVertices.reserve(activeVertices.size() + m_LightVertices.size());
		activeVertices.insert(activeVertices.end(), m_LightVertices.begin(), m_LightVertices.end());
	}
	else
	{
		activeVertices.reserve(activeVertices.size() + m_DarkVertices.size());
		activeVertices.insert(activeVertices.end(), m_DarkVertices.begin(), m_DarkVertices.end());
	}

	m_Player.Update(elapsedSec, activeVertices, m_ShadowArea);

	if (m_Player.GetPosition().y < 0.f)
	{
		m_Player.SetPosition(m_StartingPosition);
	}
}

void Game::Draw( ) const
{
	ClearBackground();

	m_Camera.Aim(m_LevelBounds.width, m_LevelBounds.height, m_Player.GetPosition());
	DrawLevel();
	utils::SetColor(Color4f{ 1.f, 0.f, 0.f, 1.f });
	utils::FillRect(m_ShadowArea);
	
	m_Player.Draw();	

	m_Camera.Reset();
}

void Game::ProcessKeyDownEvent( const SDL_KeyboardEvent & e )
{
	//std::cout << "KEYDOWN event: " << e.keysym.sym << std::endl;
}
void Game::ProcessKeyUpEvent( const SDL_KeyboardEvent& e )
{
	switch (e.keysym.sym)
	{
	case SDLK_f:
		m_Player.ShadowFlip();
		break;
	case SDLK_d:
		m_Player.Dash();
		break;
	}

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
	switch (e.button)
	{
	case SDL_BUTTON_LEFT:
		//std::cout << Vector2f(float(e.x), float(e.y)) << std::endl;
		m_Player.ShadowFlip();
		break;
	case SDL_BUTTON_RIGHT:
		m_Player.Dash();
		break;
	}
	
}

void Game::ClearBackground( ) const
{
	glClearColor( 0.4f, 0.5f, .7f, 1.0f );
	glClear( GL_COLOR_BUFFER_BIT );
}

void Game::CreateTestLevel()
{


	std::vector<Vector2f> m_Ground1{
	   Vector2f{ 0 , 94 },
	   Vector2f{ 94 , 94 },
	   Vector2f{ 94 , 38 },
	   Vector2f{ 282 , 38 },
	   Vector2f{ 282 , 97 },
	   Vector2f{ 376 , 97 },
	   Vector2f{ 376 , 43 },
	   Vector2f{ 470 , 43 },
	   Vector2f{ 470 , 114 },
	   Vector2f{ 564 , 114 },
	   Vector2f{ 564 , 0 },
	   Vector2f{ 0 , 0 },
	   Vector2f{ 0 , 94 }
	};
	std::vector<Vector2f> m_Ground2{
	Vector2f{ 752 , 0 },
	Vector2f{ 752 , 118 },
	Vector2f{ 846 , 118 },
	Vector2f{ 846 , 0 },
	Vector2f{ 752 , 0 },
	};
	std::vector<Vector2f> m_Ground3{
		Vector2f{1400, 0},
		Vector2f{1400, 280},
		Vector2f{1850, 280},
		Vector2f{1850, 0},
		Vector2f{1400, 0}
	};
	std::vector<Vector2f> m_LightPlatform{
		Vector2f{150, 200},
		Vector2f{150, 250},
		Vector2f{250, 250},
		Vector2f{250, 200},
		Vector2f{150, 200}
	};
	std::vector<Vector2f> m_DarkPlatform{
		Vector2f{620.f, 118.f},
		Vector2f{700.f, 118.f},
		Vector2f{700.f, 80.f},
		Vector2f{620.f, 80.f},
		Vector2f{620.f, 118.f}
	};
	std::vector<Vector2f> m_LightPlatform2{
		Vector2f{900, 130},
		Vector2f{900, 160},
		Vector2f{1000, 160},
		Vector2f{1000, 130},
		Vector2f{900, 130}
	};
	std::vector<Vector2f> m_LightPlatform3{
		Vector2f {1050, 160},
		Vector2f {1050, 210},
		Vector2f {1100, 210},
		Vector2f {1100, 160},
		Vector2f {1050, 160}
	};
	std::vector<Vector2f> m_DarkPlatform2{
		Vector2f {1200, 200},
		Vector2f {1200, 250},
		Vector2f {1250, 250},
		Vector2f {1250, 200},
		Vector2f {1200, 200}
	};

	m_SharedVertices.push_back(m_Ground1);
	m_SharedVertices.push_back(m_Ground2);
	m_SharedVertices.push_back(m_Ground3);
	m_LightVertices.push_back(m_LightPlatform);
	m_LightVertices.push_back(m_LightPlatform2);
	m_LightVertices.push_back(m_LightPlatform3);
	m_DarkVertices.push_back(m_DarkPlatform);
	m_DarkVertices.push_back(m_DarkPlatform2);
}

void Game::CreateLevel()
{
	SVGParser::GetVerticesFromSvgFile("level.svg", m_SharedVertices);
	CalculateLevelBounds();
}

void Game::DrawLevel() const
{
	const Color4f baseLevelColor{0.4f, 0.85f, 0.33f, 1.f};
	const Color4f lightColor{ 0.9f, 1.f, 0.6f, 1.f };
	const Color4f shadowColor{ 0.33f, 0.f, 0.66f, 1.f };

	for (int platformIdx{ 0 }; platformIdx < m_SharedVertices.size(); ++platformIdx)
	{
		utils::SetColor(baseLevelColor);
		utils::DrawPolygon(m_SharedVertices[platformIdx], true, 2.f);
	}

	if (m_Player.IsLight())
	{
		for (int platformIdx{ 0 }; platformIdx < m_LightVertices.size(); ++platformIdx)
		{
			utils::SetColor(lightColor);
			utils::DrawPolygon(m_LightVertices[platformIdx]);
		}
	}
	else
	{
		for (int platformIdx{ 0 }; platformIdx < m_DarkVertices.size(); ++platformIdx)
		{
			utils::SetColor(shadowColor);
			utils::DrawPolygon(m_DarkVertices[platformIdx]);
		}
	}
}

void Game::CalculateLevelBounds()
{
	CalculateVectorBounds(m_SharedVertices);
	CalculateVectorBounds(m_LightVertices);
	CalculateVectorBounds(m_DarkVertices);

	std::cout << m_LevelBounds.left << "  " << m_LevelBounds.bottom << "  " << m_LevelBounds.width << "  " << m_LevelBounds.height << "\n";
}

void Game::CalculateVectorBounds(std::vector<std::vector<Vector2f>> vertices)
{
	for (int platformIdx{ 0 }; platformIdx < vertices.size(); ++platformIdx)
	{
		for (int vertexIdx{ 0 }; vertexIdx < m_SharedVertices[platformIdx].size(); ++vertexIdx)
		{
			Vector2f vertex{ m_SharedVertices[platformIdx][vertexIdx] };

			if (vertex.x < m_LevelBounds.left) m_LevelBounds.left = vertex.x;
			if (vertex.x > m_LevelBounds.left + m_LevelBounds.width) m_LevelBounds.width = vertex.x - m_LevelBounds.left;
			if (vertex.y < m_LevelBounds.bottom) m_LevelBounds.bottom = vertex.y;
			if (vertex.y > m_LevelBounds.bottom + m_LevelBounds.height) m_LevelBounds.height = vertex.y - m_LevelBounds.bottom;
		}
	}
}
