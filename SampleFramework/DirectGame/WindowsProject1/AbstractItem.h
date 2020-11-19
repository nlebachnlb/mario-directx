#pragma once
#include "GameObject.h"
#include "Camera.h"

class AbstractItem : public CGameObject
{
public:
	virtual void Awake() override;
	virtual void Start() override;
	virtual void Update() override;
	virtual void LateUpdate() override;
	void PreRender() override;
	virtual void OnOverlapped(Collider2D*, Collider2D*) override;
	virtual void OnSproutCompleted();

	virtual void SproutOut();
protected:
	bool sprouting;
	Camera* mainCamera;
};

