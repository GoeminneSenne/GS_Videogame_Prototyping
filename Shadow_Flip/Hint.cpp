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

Hint::~Hint()
{
	delete m_pTexture;
}

Hint::Hint(const Hint& other)
	: m_Text{ other.m_Text }
	, m_pTexture{ new Texture(other.m_Text, "DeterminationMono.ttf", 20, Color4f()) }
	, m_DrawPos{other.m_DrawPos}
	, m_TriggerBounds(other.m_TriggerBounds)
	, m_IsActive{other.m_IsActive}
{
	
}

Hint& Hint::operator=(const Hint& other)
{
	if (this != &other)
	{
		delete m_pTexture;
		
		this->m_Text = other.m_Text;
		this->m_DrawPos = other.m_DrawPos;
		this->m_TriggerBounds = other.m_TriggerBounds;
		this->m_IsActive = other.m_IsActive;

		this->m_pTexture = new Texture(other.m_Text, "DeterminationMono.ttf", 20, Color4f());
	}
	return *this;
}

Hint::Hint(Hint&& other) noexcept
	: m_Text{std::move(other.m_Text)}
	, m_DrawPos{std::move(other.m_DrawPos)}
	, m_TriggerBounds{std::move(other.m_TriggerBounds)}
	, m_IsActive{std::move(other.m_IsActive)}
	, m_pTexture{std::move(other.m_pTexture)}
{
	other.m_pTexture = nullptr;
}

Hint& Hint::operator=(Hint&& other) noexcept
{
	if (this != &other)
	{
		delete this->m_pTexture;

		this->m_Text = std::move(other.m_Text);
		this->m_DrawPos = std::move(other.m_DrawPos);
		this->m_TriggerBounds = std::move(other.m_TriggerBounds);
		this->m_IsActive = std::move(other.m_IsActive);
		this->m_pTexture = std::move(other.m_pTexture);

		other.m_pTexture = nullptr;
	}
	return *this;
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

void Hint::Reset()
{
	m_IsActive = false;
}

const Rectf& Hint::GetTriggerBounds() const
{
	return m_TriggerBounds;
}