#pragma once
#include "GameObject.h"
#include "Camera.h"
#include "WarpUtils.h"

class WarpMark : public CGameObject
{
public:
	void Awake() override;

	void SetWarpInfo(int boundId, Vector2 dest, WarpDirection outDir, bool lockCamera);
	int GetCameraBoundId();
	Vector2 GetDestination();
	WarpDirection GetDirection();
	bool CameraLock();

private:
	int cameraBoundId;
	Vector2 destination;
	WarpDirection outDirection;
	bool lockCamera;
};

