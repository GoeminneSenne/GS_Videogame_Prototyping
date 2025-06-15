#pragma once
#include "BaseGame.h"
#include "Player.h"
#include <vector>
#include "Camera.h"
#include "Checkpoint.h"
#include "Hint.h"
class Texture;
class Game : public BaseGame
{
public:
	explicit Game( const Window& window );
	Game(const Game& other) = delete;
	Game& operator=(const Game& other) = delete;
	Game( Game&& other) = delete;
	Game& operator=(Game&& other) = delete;
	// http://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#Rh-override
	~Game();

	void Update( float elapsedSec ) override;
	void Draw( ) const override;

	// Event handling
	void ProcessKeyDownEvent( const SDL_KeyboardEvent& e ) override;
	void ProcessKeyUpEvent( const SDL_KeyboardEvent& e ) override;
	void ProcessMouseMotionEvent( const SDL_MouseMotionEvent& e ) override;
	void ProcessMouseDownEvent( const SDL_MouseButtonEvent& e ) override;
	void ProcessMouseUpEvent( const SDL_MouseButtonEvent& e ) override;

	static bool m_UsingCheats;
	static bool m_HealthCheat;
	static bool m_LensCheat;
private:

	// FUNCTIONS
	void Initialize();
	void Cleanup( );
	void ClearBackground( ) const;

	void CreateTestLevel();
	void CreateLevel();

	void DrawLevel() const;
	void DrawUI(const Vector2f& windowBottomLeft) const;

	void CalculateLevelBounds();
	void CalculateVectorBounds(std::vector<std::vector<Vector2f>> vertices);

	//MEMBERS
	Player m_Player;
	Camera m_Camera;
	Rectf m_LevelBounds;
	std::vector<std::vector<Vector2f>> m_SharedVertices;
	std::vector<std::vector<Vector2f>> m_LightVertices;
	std::vector<std::vector<Vector2f>> m_DarkVertices;
	std::vector<std::vector<Vector2f>> m_Deadzone;
	
	int m_CurrentCheckPoint;
	std::vector<Checkpoint> m_CheckPoints;
	std::vector<Hint> m_Hints;
	std::vector<Rectf> m_ShadowAreas;
};