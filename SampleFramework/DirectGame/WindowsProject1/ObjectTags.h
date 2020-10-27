#pragma once
enum class ObjectTags
{
	None,
	Player,
	SmallMario,
	BigMario,
	PowerupMario,
	FriendlyProjectiles,
	HostileProjectiles,
	Solid,
	GhostPlatform,
	Enemy
};

class TagUtils
{
public:
	static bool MarioTag(ObjectTags tag);
	static bool ProjectilesTag(ObjectTags tag);
};
