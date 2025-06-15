#pragma once
class Checkpoint
{
public:
	Checkpoint(const Vector2f& respawnPos, const Rectf& triggerBounds);

	const Vector2f& GetRespawnPosition() const;
	const Rectf& GetTriggerBounds() const;
	bool IsTriggered() const;
	void Trigger();

private:
	const Vector2f m_RespawnPosition;
	const Rectf m_TriggerBounds;
	
	bool m_IsTriggered;
};

