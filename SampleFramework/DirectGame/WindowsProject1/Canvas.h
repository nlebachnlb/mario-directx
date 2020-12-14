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
	bool operator() (const PUIElement& a, const PUIElement& b) const;
};

class Canvas
{
public:
	static void AddCanvas(std::string id, Canvas* canvas);
	static Canvas* GetCanvas(std::string id);
	static void ProcessAddRequests();
	static void UpdateAllCanvases();
	static void RenderAllCanvases();
	static void OnSceneUnloadedEvent();
	static void Clean();

	~Canvas();

	virtual void Awake();
	virtual void Start();
	virtual void Update();
	virtual void PreRender();
	virtual void Render();
	virtual void OnSceneUnload();

	void Initialize();

	void ProcessRequestsA();

	void SetActive(bool active);
	bool IsActive();

	void AddUIElement(UIElement* uiObj);

protected:
	bool enabled;
	std::set<UIElement*, Cmp> elements;
	std::vector<UIElement*> addRequests;
	std::string id;

private:
	static std::unordered_map<std::string, Canvas*> canvases;
};

