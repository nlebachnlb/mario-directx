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

	RedMushroom,
	GreenMushroom,
	RaccoonLeaf,
	FireFlower,
	Coin,

	WarpEntrance,
	WarpMark,

	HostileProjectiles,
	Solid,
	Platform,
	GhostPlatform,
	Block,
	Enemy,
	KoopasShell,
	Goomba,
	Koopa
};

class TagUtils
{
public:
	static bool MarioTag(ObjectTags tag);
	static bool EnemyTag(ObjectTags tag);
	static bool ProjectilesTag(ObjectTags tag);
	static bool StaticTag(ObjectTags tag);
	static bool PlatformTag(ObjectTags tag);
	static bool PowerupTag(ObjectTags tag);
	static bool ItemTag(ObjectTags tag);
};
