#pragma once
enum class ItemTags
{
	Coin,
	Mushroom,
	Fireplant,
	Leaf,
	PSwitch
};

struct ItemInfo
{
	ItemTags type;
	int quantity;
};

