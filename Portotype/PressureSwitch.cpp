#include "pch.h"
#include "PressureSwitch.h"
#include "utils.h"

PressureSwitch::PressureSwitch(float left, float bottom, float width, float height)
    : m_Bounds{left, bottom, width, height}
{
}

void PressureSwitch::Draw() const
{
    utils::SetColor(Color4f(0.6f, 0.6f, 0.2f, 1.f));
    utils::FillRect(m_Bounds);
}

Rectf PressureSwitch::GetBounds() const
{
    return m_Bounds;
}
