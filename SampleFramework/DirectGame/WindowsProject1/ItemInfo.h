#pragma once
enum class ItemTags
{
	Coin,
	Mushroom,
	Fireplant,
	Leaf
};

struct ItemInfo
{
	ItemTags type;
	int quantity;
};

