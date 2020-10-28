#pragma once
enum class ObjectTags
{
	None,
	Player,
	SmallMario,
	BigMario,
	PowerupMario,
	FriendlyProjectiles,
	MarioAttack,
	HostileProjectiles,
	Solid,
	GhostPlatform,
	Enemy
};

class TagUtils
{
public:
	static bool MarioTag(ObjectTags tag);
	static bool EnemyTag(ObjectTags tag);
	static bool ProjectilesTag(ObjectTags tag);
};
