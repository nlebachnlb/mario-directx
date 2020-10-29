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
};
