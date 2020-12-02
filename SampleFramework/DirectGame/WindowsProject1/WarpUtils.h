#pragma once
enum class WarpType
{
	None,
	PositionWarp,
	MapWarp
};

enum class WarpDirection
{
	None, Left, Right, Up, Down
};

class WarpUtils
{
public:
	static Vector2 ToVector(WarpDirection dir)
	{
		return Vector2(dir == WarpDirection::Left ? -1 : (dir == WarpDirection::Right ? 1 : 0),
			dir == WarpDirection::Up ? -1 : (dir == WarpDirection::Down ? 1 : 0));
	}
};