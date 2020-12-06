#pragma once
#include "UIElement.h"
#include "Font.h"

class Text : public UIElement
{
public:
	Text();

	void PreRender() override;
	void Render() override;

	void SetContent(std::string text);
	std::string GetContent();

	void SetSpacing(int spacing);

	void SetFont(Font* font);
protected:
	std::string text;
	int spacing;
	Font* font;
};

