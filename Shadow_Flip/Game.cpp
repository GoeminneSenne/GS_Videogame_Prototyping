#include "pch.h"
#include "Game.h"
#include "utils.h"
#include <iostream>
#include "SVGParser.h"

Game::Game( const Window& window ) 
	:BaseGame{ window }
	, m_Player{100.f, 100.f, 40.f, 40.f}
	//, m_Player{4020.f, 2540.f, 40.f, 40.f}
	, m_Camera{GetViewPort().width, GetViewPort().height}
	, m_StartingPosition{100.f, 100.f}
	, m_LevelBounds{0,0,0,0}
	, m_CurrentCheckPoint{0}
{
	Initialize();
}

Game::~Game( )
{
	Cleanup( );
}

void Game::Initialize( )
{
	CreateLevel();
}

void Game::Cleanup( )
{
}

void Game::Update( float elapsedSec )
{
	if (m_Player.IsLight())
	{
		m_Player.Update(elapsedSec, m_SharedVertices, m_LightVertices, m_ShadowAreas);
	}
	else
	{
		m_Player.Update(elapsedSec, m_SharedVertices, m_DarkVertices, m_ShadowAreas);
	}

	//Checkpoints controleren
	if (m_CurrentCheckPoint < m_CheckPoints.size() - 1)
	{
		if (utils::IsOverlapping(m_Player.GetBounds(), m_CheckPoints[m_CurrentCheckPoint + 1].GetTriggerBounds()))
		{
			if (not m_CheckPoints[m_CurrentCheckPoint + 1].IsTriggered())
			{
				++m_CurrentCheckPoint;
				m_CheckPoints[m_CurrentCheckPoint].Trigger();
				m_Player.ResetLensTime();

				std::cout << "Triggered checkpoint " << m_CurrentCheckPoint << "\n";
			}
		}
	}




	//Doodgaan
	if (m_Player.GetPosition().y < 0.f)
	{
		m_Player.Respawn(m_CheckPoints[m_CurrentCheckPoint].GetRespawnPosition());
	}
}

void Game::Draw( ) const
{
	ClearBackground();

	Vector2f windowBottomLeft = m_Camera.Aim(m_LevelBounds.width, m_LevelBounds.height, m_Player.GetPosition());
	DrawLevel();
	utils::SetColor(Color4f{ 1.f, 0.f, 0.f, 1.f });
	
	m_Player.Draw();	
	DrawLensMeter(windowBottomLeft);

	////////////////////VOORLOPIG
	for (const Checkpoint& cp : m_CheckPoints)
	{
		utils::DrawRect(cp.GetTriggerBounds());
	}



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
	case SDLK_r:
		m_Player.Respawn(m_CheckPoints[m_CurrentCheckPoint].GetRespawnPosition());
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
	SVGParser::GetVerticesFromSvgFile("level_shared.svg", m_SharedVertices);
	SVGParser::GetVerticesFromSvgFile("level_light.svg", m_LightVertices);
	SVGParser::GetVerticesFromSvgFile("level_dark.svg", m_DarkVertices);
	CalculateLevelBounds();

	m_ShadowAreas.push_back(Rectf(1780.f, 1202.f, 190.f, 5.f));
	m_ShadowAreas.push_back(Rectf(3530.f, 1623.f, 170.f, 5.f));
	m_ShadowAreas.push_back(Rectf(3880, 1701, 170.f, 5.f));
	m_ShadowAreas.push_back(Rectf(3870, 2235, 170.f, 5.f));

	m_CheckPoints.push_back(Checkpoint(Vector2f(100.f, 100.f), Rectf()));
	m_CheckPoints.push_back(Checkpoint(Vector2f(800.f, 537.f), Rectf(790.f, 537.f, 30.f, 300.f)));
	m_CheckPoints.push_back(Checkpoint(Vector2f(2035.f, 1205.f), Rectf(2035.f, 1205.f, 40.f, 300.f)));
	m_CheckPoints.push_back(Checkpoint(Vector2f(4025.f , 1704.f), Rectf(4020.f, 1704.f, 80.f, 200.f)));
	m_CheckPoints.push_back(Checkpoint(Vector2f(4035.f , 2540.f), Rectf(4030.f, 2540.f, 60.f, 300.f)));
}

void Game::DrawLevel() const
{
	const Color4f baseLevelColor{0.4f, 0.85f, 0.33f, 1.f};
	const Color4f lightColor{ 0.9f, 1.f, 0.6f, 1.f };
	const Color4f shadowColor{ 0.33f, 0.f, 0.66f, 1.f };
	const Color4f shadowAreaColor{ 1.f, 0.f, 0.f, 1.f };

	for (int platformIdx{ 0 }; platformIdx < m_SharedVertices.size(); ++platformIdx)
	{
		utils::SetColor(baseLevelColor);
		utils::DrawPolygon(m_SharedVertices[platformIdx], true, 2.f);
		//utils::FillPolygon(m_SharedVertices[platformIdx]);
	}

	if (m_Player.IsLight() || m_Player.IsUsingLens())
	{
		for (int platformIdx{ 0 }; platformIdx < m_LightVertices.size(); ++platformIdx)
		{
			utils::SetColor(lightColor);
			utils::DrawPolygon(m_LightVertices[platformIdx]);
		}
	}
	
	if(not m_Player.IsLight() ||  m_Player.IsUsingLens())
	{
		for (int platformIdx{ 0 }; platformIdx < m_DarkVertices.size(); ++platformIdx)
		{
			utils::SetColor(shadowColor);
			utils::DrawPolygon(m_DarkVertices[platformIdx]);
		}
	}

	utils::SetColor(shadowAreaColor);
	for (const Rectf& shadowArea : m_ShadowAreas)
	{
		utils::FillRect(shadowArea);
	}
}

void Game::DrawLensMeter(const Vector2f& windowBottomLeft) const
{
	const float BORDER_MARGIN{ 10.f };
	const float METER_WIDTH{ 200.f }, METER_HEIGHT{ 30.f };
	const Color4f meterColor{ 0.f, 0.f, 0.7f, 1.f };

	Vector2f drawPos{ windowBottomLeft };
	drawPos.x += BORDER_MARGIN;
	drawPos.y += GetViewPort().height - METER_HEIGHT - BORDER_MARGIN;

	float currentWidth{ m_Player.GetLensTime() / m_Player.GetLensMax() };

	utils::SetColor(meterColor);
	utils::FillRect(drawPos, METER_WIDTH * currentWidth, METER_HEIGHT);
	utils::SetColor(Color4f(0.f, 0.f, 0.f, 1.f));
	utils::DrawRect(drawPos, METER_WIDTH, METER_HEIGHT, 6.f);
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
