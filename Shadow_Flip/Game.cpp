#include "pch.h"
#include "Game.h"
#include "utils.h"
#include <iostream>
#include "SVGParser.h"
#include "Texture.h"

bool Game::m_UsingCheats{ false };
bool Game::m_HealthCheat{ false };
bool Game::m_LensCheat{ false };

Game::Game( const Window& window ) 
	:BaseGame{ window }
	, m_Player{100.f, 100.f, 40.f, 40.f}
	//, m_Player{ 5370, 3090, 40.f, 40.f}
	, m_Camera{GetViewPort().width, GetViewPort().height}
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
	if (m_Player.GetLives() < 0)
	{
		return;
	}


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
			}
		}
	}

	//Check hints
	for (Hint& hint : m_Hints)
	{
		if (utils::IsOverlapping(m_Player.GetBounds(), hint.GetTriggerBounds()))
		{
			hint.Activate();
		}
	}


	//Doodgaan
	const float playerSize{ m_Player.GetBounds().width / 2.f };
	Circlef playerCircle{ m_Player.GetPosition(), playerSize };
	playerCircle.center.x += playerSize;
	playerCircle.center.y += playerSize;

	if (m_Player.GetPosition().y < 0.f || utils::IsOverlapping(m_Deadzone[0], playerCircle))
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
	


	////////////////////VOORLOPIG

	for (const Checkpoint& cp : m_CheckPoints)
	{
		if (cp.GetTriggerBounds().width != 0.f)
		{

			utils::DrawRect(cp.GetTriggerBounds());
			cp.Draw();
		}
	}

	utils::DrawPolygon(m_Deadzone[0]);

	for (const Hint& hint : m_Hints)
	{
		hint.Draw();
	}
	/////////////////////////EINDE VOORLOPIG


	m_Player.Draw();	
	DrawUI(windowBottomLeft);

	m_Camera.Reset();
}

void Game::ProcessKeyDownEvent( const SDL_KeyboardEvent & e )
{
	//std::cout << "KEYDOWN event: " << e.keysym.sym << std::endl;
	if (e.keysym.sym == SDLK_v)
	{
		if (utils::IsOverlapping(m_Player.GetBounds(), Rectf(1140, 135, 110, 30)))
			m_Hints[3].Activate(); //Hardcoded for now
	}
}
void Game::ProcessKeyUpEvent( const SDL_KeyboardEvent& e )
{
	switch (e.keysym.sym)
	{
	case SDLK_f:
		if(m_Player.GetLives() >= 0) m_Player.ShadowFlip();
		break;
	case SDLK_d:
		if (m_Player.GetLives() >= 0) m_Player.Dash();
		break;
	case SDLK_r:
		if (m_Player.GetLives() >= 0) m_Player.Respawn(m_CheckPoints[m_CurrentCheckPoint].GetRespawnPosition());
		break;
	case SDLK_MINUS:
		if (not m_UsingCheats) std::cout << "Enabled cheats\n";
		m_UsingCheats = true;
		break;
	case SDLK_0:
		if (m_UsingCheats)
		{
			if (not m_HealthCheat) std::cout << "Activated health cheat\n";
			if (m_Player.GetLives() <= 0) m_Player.SetLives(999);
			m_HealthCheat = true;
		}
		break;
	case SDLK_9:
		if (m_UsingCheats)
		{
			if (not m_LensCheat) std::cout << "Activated lens cheat\n";
			m_Player.ResetLensTime();
			m_LensCheat = true;
		}
		break;
	case SDLK_ESCAPE:
		if(m_UsingCheats)
		{
			m_CurrentCheckPoint = 0;
			m_Player.SetPosition(m_CheckPoints[0].GetRespawnPosition());
		 }
		break;
	case SDLK_1:
	case SDLK_2:
	case SDLK_3:
	case SDLK_4:
		if (m_UsingCheats)
		{
			m_CurrentCheckPoint = e.keysym.sym - 48;
			m_Player.SetPosition(m_CheckPoints[m_CurrentCheckPoint].GetRespawnPosition());
		}
		break;
	case SDLK_RETURN:
		if (m_Player.GetLives() < 0) ResetLevel();
		break;
	default:
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
	SVGParser::GetVerticesFromSvgFile("level_deadzone.svg", m_Deadzone);
	CalculateLevelBounds();

	m_ShadowAreas.push_back(Rectf(1780.f, 1202.f, 190.f, 5.f));
	m_ShadowAreas.push_back(Rectf(3530.f, 1623.f, 170.f, 5.f));
	m_ShadowAreas.push_back(Rectf(3880, 1701, 170.f, 5.f));
	m_ShadowAreas.push_back(Rectf(3870, 2235, 170.f, 5.f));

	m_CheckPoints.push_back(Checkpoint(Vector2f(100.f, 100.f), Rectf()));
	m_CheckPoints.push_back(Checkpoint(Vector2f(800.f, 537.f), Rectf(790.f, 537.f, 30.f, 300.f)));
	m_CheckPoints.push_back(Checkpoint(Vector2f(2035.f, 1205.f), Rectf(2035.f, 1205.f, 40.f, 300.f)));
	m_CheckPoints.push_back(Checkpoint(Vector2f(4025.f , 1704.f), Rectf(4020.f, 1704.f, 80.f, 200.f)));
	m_CheckPoints.push_back(Checkpoint(Vector2f(4035.f , 2530.f), Rectf(4030.f, 2530.f, 60.f, 300.f)));

	m_Hints.push_back(Hint("Use <- -> to move & SPACE to jump", Vector2f(35, 250), Rectf(0, 70, 300, 30)));
	m_Hints.push_back(Hint("Press the V key to", Vector2f(1100, 250), Rectf(1140, 135, 110, 30)));
	m_Hints.push_back(Hint("use your lens ability", Vector2f(1090, 225), Rectf(1140, 135, 110, 30)));
	m_Hints.push_back(Hint("Press F to change world", Vector2f(1150, 300), Rectf()));
	m_Hints.push_back(Hint("In shadow form press D to dash", Vector2f(1400, 570), Rectf(1650, 390, 110, 20)));
	m_Hints.push_back(Hint("In light mode, hold UP while jumping", Vector2f(470, 800), Rectf(270, 537, 200, 20)));
	m_Hints.push_back(Hint("to perform a super jump", Vector2f(530, 775), Rectf(270, 537, 200, 20)));
	m_Hints.push_back(Hint("Hold SPACE to glide", Vector2f(390, 970), Rectf(145, 885, 95, 10)));
	m_Hints.push_back(Hint("Press DOWN to enter your shadow", Vector2f(1520, 1400), Rectf(1750, 1200, 95, 20)));
	m_Hints.push_back(Hint("Congratulations", Vector2f(5410, 3140), Rectf(5370, 3010, 240, 30)));
	m_Hints.push_back(Hint("you reached the end", Vector2f(5390, 3115), Rectf(5370, 3010, 240, 30)));


}

void Game::ResetLevel()
{
	std::cout << "Restart";

	m_CurrentCheckPoint = 0;
	for (Checkpoint& cp : m_CheckPoints)
	{
		cp.Reset();
	}
	for (Hint& hint : m_Hints)
	{
		hint.Reset();
	}

	m_Player.Reset(m_CheckPoints[m_CurrentCheckPoint].GetRespawnPosition());
	
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

void Game::DrawUI(const Vector2f& windowBottomLeft) const
{
	//Calculate Topleft
	const float BORDER_MARGIN{ 10.f };
	Vector2f drawPos{ windowBottomLeft };
	drawPos.x += BORDER_MARGIN;
	drawPos.y += GetViewPort().height - BORDER_MARGIN;

	//DrawHealth
	const float CIRCLE_RADIUS{ 25.f};
	drawPos.x += CIRCLE_RADIUS;
	drawPos.y -= CIRCLE_RADIUS;
	
	utils::SetColor(Color4f(0.9f, 0.1f, 0.1f, 1.f));
	utils::FillEllipse(drawPos, CIRCLE_RADIUS, CIRCLE_RADIUS);

	std::string lives{ std::to_string(m_Player.GetLives()) };
	if (lives == "-1") lives = "X";
	Texture livesTex{ lives, "DeterminationMono.ttf", 40, Color4f() };
	livesTex.Draw(Vector2f(drawPos.x - livesTex.GetWidth()/2.f, drawPos.y - livesTex.GetHeight()/2.f));

	//DrawMeter
	const float METER_WIDTH{ 200.f }, METER_HEIGHT{ 30.f };
	const Color4f meterColor{ 0.f, 0.f, 0.7f, 1.f };
	float currentWidth{ m_Player.GetLensTime() / m_Player.GetLensMax() };
	drawPos.x += CIRCLE_RADIUS + BORDER_MARGIN;
	drawPos.y -= METER_HEIGHT / 2.f;

	utils::SetColor(meterColor);
	utils::FillRect(drawPos, METER_WIDTH * currentWidth, METER_HEIGHT);
	utils::SetColor(Color4f(0.f, 0.f, 0.f, 1.f));
	utils::DrawRect(drawPos, METER_WIDTH, METER_HEIGHT, 6.f);

	//Draw Game over screen if dead
	
	if (m_Player.GetLives() < 0)
	{
		Rectf window{ windowBottomLeft.x, windowBottomLeft.y, GetViewPort().width, GetViewPort().height};
		utils::SetColor(Color4f(0.f, 0.f, 0.f, 0.6f));
		utils::FillRect(window);

		Texture gameOver{ "GAME OVER", "DeterminationMono.ttf", 100, Color4f(1.f, 1.f, 1.f, 1.f) };
		drawPos.x = windowBottomLeft.x + (GetViewPort().width - gameOver.GetWidth()) / 2.f;
		drawPos.y = windowBottomLeft.y + GetViewPort().height / 2.f;
		gameOver.Draw(drawPos);

		Texture retry{ "Press ENTER to retry", "DeterminationMono.ttf", 50, Color4f(1.f, 1.f, 1.f,1.f) };
		drawPos.x += (gameOver.GetWidth() - retry.GetWidth()) / 2.f;
		drawPos.y -= gameOver.GetHeight() / 2.f + retry.GetHeight();
		retry.Draw(drawPos);
	}
}


void Game::CalculateLevelBounds()
{
	CalculateVectorBounds(m_SharedVertices);
	CalculateVectorBounds(m_LightVertices);
	CalculateVectorBounds(m_DarkVertices);
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
