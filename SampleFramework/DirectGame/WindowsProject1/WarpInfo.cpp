#include "WarpInfo.h"

WarpInfo WarpInfo::Empty()
{
	return WarpInfo(WarpType::None, WarpDirection::None, VectorZero(), VectorZero(), false);
}

WarpInfo::WarpInfo(WarpType type, WarpDirection direction, Vector2 src, Vector2 dst, bool hasTransition)
{
	this->type = type;
	this->direction = direction;
	this->srcPosition = src;
	this->dstPosition = dst;
	this->hasTransition = hasTransition;
}
