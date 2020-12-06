#pragma once
#include "UIElement.h"
#include "Font.h"

enum class TextAlignment
{
	Left, Right
};

class Text : public UIElement
{
public:
	Text();

	void PreRender() override;
	void Render() override;

	void SetContent(std::string text);
	std::string GetContent();

	void SetSpacing(int spacing);
	void SetAlignment(TextAlignment alignment);

	void SetFont(Font* font);
protected:
	std::string text;
	int spacing;
	Font* font;
	TextAlignment alignment;
};

