#pragma once
#include "Transform.h"
#include "UIElement.h"
#include <set>
#include <vector>
#include <unordered_map>

class UIElement;
typedef UIElement* PUIElement;

struct Cmp
{
	bool operator() (const PUIElement& a, const PUIElement& b);
};

class Canvas
{
public:
	static void AddCanvas(std::string id, Canvas* canvas);
	static Canvas* GetCanvas(std::string id);
	static void ProcessAddRequests();
	static void Clean();

	~Canvas();

	virtual void Awake();
	virtual void Start();
	virtual void Update();
	virtual void PreRender();
	virtual void Render();

	void ProcessRequestsA();

	void AddUIElement(UIElement* uiObj);

protected:
	bool enabled;
	std::set<UIElement*, Cmp> elements;
	std::vector<UIElement*> addRequests;
	std::string id;

private:
	static std::unordered_map<std::string, Canvas*> canvases;
};

