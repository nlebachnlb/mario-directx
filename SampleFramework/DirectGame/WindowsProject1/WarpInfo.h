#pragma once
#include "Transform.h"
enum class WarpType
{
	None,
	WarpIn,
	WarpOut,
	WarpInOut
};

enum class WarpDirection
{
	None, Left, Right, Up, Down
};

struct WarpInfo
{
	WarpType type;
	WarpDirection direction;
	Vector2 srcPosition, dstPosition;
	bool hasTransition;

	static WarpInfo Empty();
	WarpInfo(WarpType type, WarpDirection direction, Vector2 src, Vector2 dst, bool hasTransition);
};

