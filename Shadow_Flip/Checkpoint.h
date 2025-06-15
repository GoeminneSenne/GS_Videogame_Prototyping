#pragma once
#include "Texture.h"
class Checkpoint
{
public:
	Checkpoint(const Vector2f& respawnPos, const Rectf& triggerBounds);

	void Draw() const;

	const Vector2f& GetRespawnPosition() const;
	const Rectf& GetTriggerBounds() const;
	bool IsTriggered() const;
	void Trigger();

private:
	const Vector2f m_RespawnPosition;
	const Rectf m_TriggerBounds;
	
	Texture* m_Text;

	bool m_IsTriggered;
};

