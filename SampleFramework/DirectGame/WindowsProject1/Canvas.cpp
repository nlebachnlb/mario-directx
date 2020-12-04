#include "Canvas.h"

std::unordered_map<std::string, Canvas*> Canvas::canvases;

void Canvas::AddCanvas(std::string id, Canvas* canvas)
{
	canvases.insert(std::make_pair(id, canvas));
}

Canvas* Canvas::GetCanvas(std::string id)
{
	if (canvases.find(id) != canvases.end())
		return canvases.at(id);
	return nullptr;
}

void Canvas::ProcessAddRequests()
{
	for (auto x : canvases)
	{
		auto canvas = x.second;
		canvas->ProcessRequestsA();
	}
}

void Canvas::UpdateAllCanvases()
{
	for (auto x : canvases)
	{
		auto canvas = x.second;
		if (canvas->IsActive()) 
			canvas->Update();
	}
}

void Canvas::RenderAllCanvases()
{
	for (auto x : canvases)
	{
		auto canvas = x.second;
		if (canvas->IsActive())
			canvas->Update();
	}
}

void Canvas::Clean()
{

}

Canvas::~Canvas()
{
	for (auto e : elements) delete e;
}

void Canvas::Awake()
{
	for (auto e : elements)
		e->Awake();
}

void Canvas::Start()
{
	for (auto e : elements)
		e->Start();
}

void Canvas::Update()
{
	if (!enabled) return;
	for (auto e : elements)
		if (e->IsActive()) e->Update();
}

void Canvas::PreRender()
{
	if (!enabled) return;
	for (auto e : elements)
		if (e->IsActive()) e->PreRender();
}

void Canvas::Render()
{
	if (!enabled) return;
	for (auto e : elements)
		e->Render();
}

void Canvas::ProcessRequestsA()
{
	if (addRequests.size() > 0)
	{
		for (auto e : addRequests)
			AddUIElement(e);
		addRequests.clear();
	}
}

void Canvas::SetActive(bool active)
{
	this->enabled = active;
}

bool Canvas::IsActive()
{
	return this->enabled;
}

void Canvas::AddUIElement(UIElement* uiObj)
{
	addRequests.push_back(uiObj);
}

bool Cmp::operator()(const PUIElement& a, const PUIElement& b)
{
	return a->GetRenderOrder() < b->GetRenderOrder();
}
