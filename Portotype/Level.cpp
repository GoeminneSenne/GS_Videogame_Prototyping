#include "pch.h"
#include "Level.h"
#include "utils.h"

Level::Level()
{
	m_LevelVertices.push_back(std::vector<Vector2f>{Vector2f(100.f, 100.f), Vector2f(600.f, 100.f), Vector2f(600.f, 350.f), Vector2f(100.f, 350.f)});
	m_LevelVertices.push_back(std::vector<Vector2f>{Vector2f(100.f, 350.f), Vector2f(600.f, 350.f), Vector2f(600.f, 450.f), Vector2f(100.f, 450.f)});

	m_Switches.push_back(PressureSwitch{ 400.f, 120.f, 50.f, 50.f });
	m_Switches.push_back(PressureSwitch{ 300.f, 370.f, 30.f, 30.f });

	m_Doors.push_back(Door{ &m_Switches[0], false, Vector2f(500.f, 100.f), Vector2f(500.f, 350.f)});
	m_Doors.push_back(Door{ &m_Switches[0], true, Vector2f(200.f, 350.f), Vector2f(200.f, 450.f) });
	m_Doors.push_back(Door{ &m_Switches[1], true, Vector2f(550.f, 100.f), Vector2f(550.f, 350.f)});

	m_EndCollectible = Circlef{ 575.f, 225.f, 15.f };
}

void Level::Update(const Player& player1, const Player& player2)
{
	for (int idx{0}; idx < m_Switches.size(); ++idx)
	{
		m_Switches[idx].CheckCollision(player1, player2);
	}
	for (int doorIdx{ 0 }; doorIdx < m_Doors.size(); ++doorIdx)
	{
		m_Doors[doorIdx].Update();
	}
}

void Level::Draw() const
{
	for (const PressureSwitch& presSwitch : m_Switches)
	{
		presSwitch.Draw();
	}
	for (const Door& door : m_Doors)
	{
		door.Draw();
	}

	utils::SetColor(Color4f{ 0.f, 1.f, 1.f, 1.f });
	utils::FillEllipse(m_EndCollectible.center, m_EndCollectible.radius, m_EndCollectible.radius);

	//Draw level vertices
	utils::SetColor(Color4f(1.f, 0.f, 0.f, 1.f));
	for (int idx{ 0 }; idx < m_LevelVertices.size(); ++idx)
	{
		utils::DrawPolygon(m_LevelVertices[idx], true);
	}
}

std::vector<std::vector<Vector2f>> Level::GetLevelVertices() const
{
	return m_LevelVertices;
}

std::vector<PressureSwitch> Level::GetSwitches() const
{
	return m_Switches;
}

std::vector<Door> Level::GetDoors() const
{
	return m_Doors;
}

Circlef Level::GetEndCollectible() const
{
	return m_EndCollectible;
}
