#include "Holdable.h"

Holdable::Holdable()
{
	holder = nullptr;
}

void Holdable::PassToHolder(GameObject holder)
{
	this->holder = holder;
}

GameObject Holdable::GetHolder()
{
	return this->holder;
}

bool Holdable::IsHeld()
{
	return holder != nullptr;
}

void Holdable::Release()
{
	holder = nullptr;
	OnRelease();
}

void Holdable::SetHoldableFacing(int direction)
{
	this->holdableFacing = direction;
}
