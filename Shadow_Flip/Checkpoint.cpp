#include "pch.h"
#include "Checkpoint.h"

Checkpoint::Checkpoint(const Vector2f& respawnPos, const Rectf& triggerBounds)
	: m_RespawnPosition{respawnPos}
	, m_TriggerBounds{triggerBounds}
	, m_IsTriggered{false}
{
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
