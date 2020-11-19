#pragma once
#include "GameObject.h"
class AbstractItem : public CGameObject
{
public:
	virtual void Awake() override;
	virtual void Start() override;
	virtual void Update() override;
	void PreRender() override;
	virtual void OnOverlapped(Collider2D*, Collider2D*) override;
	virtual void OnSproutCompleted();

	virtual void SproutOut();
protected:
	bool sprouting;
};

