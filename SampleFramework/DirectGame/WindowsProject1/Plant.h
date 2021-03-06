#pragma once
#include "AbstractEnemy.h"

enum class PlantState
{
	Reveal,
	Hidden
};

class Plant : public AbstractEnemy
{
public:
	virtual void Awake() override;
	virtual void Start() override;
	virtual void Movement() override;
	virtual void OnDead(bool oneHit) override;
	virtual void PreRender() override;
	virtual void OnOverlapped(Collider2D* self, Collider2D* other) override;

	virtual void OnRevealed();
	virtual void OnHidden();

protected:
	void TrackPlayerPosition();
	virtual Vector2 GetBoxSize() = 0;
	PlantState state;
	int movementPhase;
	int waitTime, timer;
	float speed;
	float hideDistance;
};

