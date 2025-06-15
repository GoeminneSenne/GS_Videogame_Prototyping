#include "pch.h"
#include "Checkpoint.h"

Checkpoint::Checkpoint(const Vector2f& respawnPos, const Rectf& triggerBounds)
	: m_RespawnPosition{respawnPos}
	, m_TriggerBounds{triggerBounds}
	, m_IsTriggered{false}
	, m_Text{ new Texture("Checkpoint", "DeterminationMono.ttf", 30, Color4f())}
{
}

void Checkpoint::Draw() const
{
	Vector2f drawPos{ m_RespawnPosition };
	drawPos.x += 20.f;
	drawPos.y += 100.f;

	drawPos.x -= m_Text->GetWidth() / 2.f;
	drawPos.y -= m_Text->GetHeight() / 2.f;

	m_Text->Draw(drawPos);
}

const Vector2f& Checkpoint::GetRespawnPosition() const
{
	return m_RespawnPosition;
}

const Rectf& Checkpoint::GetTriggerBounds() const
{
	return m_TriggerBounds;
}

bool Checkpoint::IsTriggered() const
{
	return m_IsTriggered;
}

void Checkpoint::Trigger()
{
	m_IsTriggered = true;
}

void Checkpoint::Reset()
{
	m_IsTriggered = false;
}
