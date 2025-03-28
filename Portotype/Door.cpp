#include "pch.h"
#include "Door.h"
#include "PressureSwitch.h"
#include "utils.h"

Door::Door(PressureSwitch* linkedSwitch, bool isHolding, const Vector2f& start, const Vector2f& end)
	: m_pLinkedSwitch{linkedSwitch}
	, m_IsHoldingSwitch{isHolding}
	, m_IsOpened{false}
{
	m_Vertices.push_back(start);
	m_Vertices.push_back(end);
}

void Door::Update()
{
	if (!m_IsHoldingSwitch)
	{
		if (!m_IsOpened)
		{
			m_IsOpened = m_pLinkedSwitch->IsActivated();
		}
	}
	else
	{
		m_IsOpened = m_pLinkedSwitch->IsActivated();
	}
}

void Door::Draw() const
{
	if (!m_IsOpened)
	{
		utils::SetColor(Color4f(1.f, 1.f, 0.f, 1.f));
		utils::DrawPolygon(m_Vertices, false);
	}
}

bool Door::isHoldingSwitch() const
{
	return m_IsHoldingSwitch;
}

bool Door::isOpened() const
{
	return m_IsOpened;
}

std::vector<Vector2f> Door::GetVertices() const
{
	return m_Vertices;
}
