#pragma once
#include "BaseGame.h"
#include "Player.h"
#include <vector>
#include "Camera.h"
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

private:

	// FUNCTIONS
	void Initialize();
	void Cleanup( );
	void ClearBackground( ) const;

	void DrawLevel() const;

	void CalculateLevelBounds();
	void CalculateVectorBounds(std::vector<std::vector<Vector2f>> vertices);

	//MEMBERS
	Player m_Player;
	Camera m_Camera;
	std::vector<std::vector<Vector2f>> m_SharedVertices;
	std::vector<std::vector<Vector2f>> m_LightVertices;
	std::vector<std::vector<Vector2f>> m_DarkVertices;
	Rectf m_LevelBounds;

	Rectf m_ShadowArea;

	const Vector2f m_StartingPosition;
};