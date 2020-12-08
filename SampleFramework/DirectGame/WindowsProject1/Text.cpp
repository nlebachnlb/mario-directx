#include "Text.h"
#include "Mathf.h"

Text::Text()
{
	font = nullptr;
	spacing = -3;
	alignment = TextAlignment::Left;
}

void Text::PreRender()
{
}

void Text::Render()
{
	if (font == nullptr) return;

	int start, end, step;
	switch (alignment)
	{
	case TextAlignment::Left:
		start = 0, end = text.length() - 1, step = 1;
		break;
	case TextAlignment::Right:
		start = text.length() - 1, end = 0, step = -1;
		break;
	}

	int x = 0;
	for (int i = start; (step > 0 ? i <= end : i >= end); i += step)
	{
		auto ch = text.at(i);
		auto visual = font->GetChar(ch);
		auto piv = (alignment == TextAlignment::Left ? 0 : visual->GetSpriteWidth());
		visual->Draw(rectTransform.Position.x + x, rectTransform.Position.y, piv, 0);

		auto charStep = 0;
		if (alignment == TextAlignment::Left)
			charStep = (visual->GetSpriteWidth() + spacing);
		else if (alignment == TextAlignment::Right && i > end)
			charStep = -(visual->GetSpriteWidth() + spacing);

		x += charStep;
	}
}

void Text::SetContent(std::string text)
{
	//OutputDebugStringW(ToLPCWSTR(text + "\n"));
	this->text = text;
	for (auto c : text) c = toupper(c);
}

std::string Text::GetContent()
{
	return this->text;
}

void Text::SetSpacing(int spacing)
{
	this->spacing = spacing;
}

void Text::SetAlignment(TextAlignment alignment)
{
	this->alignment = alignment;
}

void Text::SetFont(Font* font)
{
	this->font = font;
}
