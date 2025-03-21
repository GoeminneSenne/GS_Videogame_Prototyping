#include "pch.h"
#include "PressureSwitch.h"
#include "utils.h"
#include "Player.h"

PressureSwitch::PressureSwitch(float left, float bottom, float width, float height)
    : m_Bounds{left, bottom, width, height}
    , m_IsActivated{false}
{
}

void PressureSwitch::Draw() const
{
    if (m_IsActivated)
    {
        utils::SetColor(Color4f(0.f, 1.f, 0.f, 1.f));
    }
    else
    {
        utils::SetColor(Color4f(0.6f, 0.6f, 0.2f, 1.f));
    }
    utils::FillRect(m_Bounds);
}

void PressureSwitch::CheckCollision(const Player& player)
{
    if (utils::IsOverlapping(m_Bounds, player.GetBounds()))
    {
        m_IsActivated = true;
    }
    else
    {
        m_IsActivated = false;
    }
}

Rectf PressureSwitch::GetBounds() const
{
    return m_Bounds;
}

bool PressureSwitch::IsActivated() const
{
    return m_IsActivated;
}
