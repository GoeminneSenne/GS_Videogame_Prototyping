#pragma once
class Texture;
class Hint
{
public:
	Hint(const std::string& text, const Vector2f& drawPos, const Rectf& triggerBounds);

	void Draw() const;

	void Activate();
	const Rectf& GetTriggerBounds() const;
private:
	Texture* m_pTexture;
	std::string m_Text;
	Vector2f m_DrawPos;
	Rectf m_TriggerBounds;
	bool m_IsActive;
};

