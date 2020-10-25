#pragma once
// Effector tells the physics system which side of collider should receive collision
enum class Effector2D
{
	Full, // 4 sides of box: default
	Left,
	Top,
	Right,
	Bottom
};