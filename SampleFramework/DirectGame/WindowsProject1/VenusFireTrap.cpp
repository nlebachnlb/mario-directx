#include "VenusFireTrap.h"
#include "Mathf.h"

void VenusFireTrap::Start()
{
	Plant::Start();

	waitTime = VENUS_WAIT_TIME;
	speed = VENUS_SPEED;
	hideDistance = VENUS_HIDE_DISTANCE;
}

void VenusFireTrap::PreRender()
{
	std::string dir = verticalDirection == 1 ? "HeadDown" : "HeadUp";
	switch (movementPhase)
	{
	case 0:
	case 2:
		if (currentState.compare("Reveal" + dir) != 0) SetState("Reveal" + dir);
		break;
	case 1:
	case 3:
		if (currentState.compare("Idle" + dir) != 0) SetState("Idle" + dir);
		break;
	}
	SetScale(Vector2(-direction, 1));
}

void VenusFireTrap::LateUpdate()
{
	if (movementPhase <= 1) UpdateDirection();
}

void VenusFireTrap::OnRevealed()
{
}

Vector2 VenusFireTrap::GetBoxSize()
{
	return VENUS_BBOX;
}

void VenusFireTrap::UpdateDirection()
{
	AbstractEnemy::UpdateDirection();
	verticalDirection = player != nullptr ? 
		Mathf::Sign(player->GetTransform().Position.y + visualRelativePosition.y - 0.25f * GetBoxSize().y - transform.Position.y) : 1;
}
