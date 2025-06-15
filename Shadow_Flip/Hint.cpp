#include "pch.h"
#include "Hint.h"
#include "Texture.h"

Hint::Hint(const std::string& text, const Vector2f& drawPos, const Rectf& triggerBounds)
	: m_Text{text}
	, m_pTexture{ new Texture(text, "DeterminationMono.ttf", 20, Color4f())}
	, m_DrawPos{drawPos}
	, m_TriggerBounds{triggerBounds}
	, m_IsActive{false}
{
}

void Hint::Draw() const
{
	if (m_IsActive)
	{
		m_pTexture->Draw(m_DrawPos);
	}
}

void Hint::Activate()
{
	m_IsActive = true;
}

const Rectf& Hint::GetTriggerBounds() const
{
	return m_TriggerBounds;
}