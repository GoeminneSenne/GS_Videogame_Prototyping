#pragma once
#include <vector>
#include "PressureSwitch.h"
#include "Door.h"
class Player;
class Level
{
public:
	Level();

	void Update(const Player& player1, const Player& player2);
	void Draw() const;

	std::vector<std::vector<Vector2f>> GetLevelVertices() const;
	std::vector<PressureSwitch> GetSwitches() const;
	std::vector<Door> GetDoors() const;
	Circlef GetEndCollectible() const;
private:
	std::vector<std::vector<Vector2f>> m_LevelVertices;
	std::vector<PressureSwitch> m_Switches;
	std::vector<Door> m_Doors;

	Circlef m_EndCollectible;
};

