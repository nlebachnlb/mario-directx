#pragma once
enum class ItemTags
{
	Coin,
	Mushroom,
	Green1UP,
	Fireplant,
	Leaf,
	PSwitch
};

struct ItemInfo
{
	ItemTags type;
	int quantity;
};

