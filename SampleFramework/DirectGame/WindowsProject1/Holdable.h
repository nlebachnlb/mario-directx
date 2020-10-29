#pragma once
#include "GameObject.h"
class Holdable
{
public:
	Holdable();

	void PassToHolder(GameObject holder);
	GameObject GetHolder();
	bool IsHeld();
	void Release();
	void SetHoldableFacing(int direction);

	virtual Vector2 GetColliderBox() = 0; 
	virtual void SetHoldablePosition(Vector2 position) = 0;
	virtual void OnRelease() = 0;
protected:
	GameObject holder;
	int holdableFacing;
};

