#pragma once

enum class ItemCardType
{
	Empty,
	Mushroom,
	Fireflower,
	Superstar
};

struct ItemCard
{
	ItemCardType item;
};