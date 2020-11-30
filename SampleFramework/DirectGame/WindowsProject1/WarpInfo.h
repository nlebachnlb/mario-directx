#pragma once
#include "Transform.h"
#include "WarpUtils.h"

struct WarpInfo
{
	WarpType type;
	WarpDirection direction;
	Vector2 srcPosition, dstPosition;
	bool hasTransition;

	static WarpInfo Empty();
	WarpInfo(WarpType type, WarpDirection direction, Vector2 src, Vector2 dst, bool hasTransition);
};

