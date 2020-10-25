#pragma once
#include "CMario.h"

const int RACCOON_FLOAT_TIME = 400; // miliseconds
const int RACCOON_FLY_TIME = 4000; // miliseconds = 4 seconds

class RaccoonMario : public CMario
{
public:
	void Awake() override;
	void Start() override;
	void OnAnimationEnd() override;
	void OnKeyDown(int keyCode) override;
protected:
	void InitAnimations() override;
	void MovementAnimation() override;
	void JumpingAnimation() override;
	void LateUpdate() override;
private:
	bool attacking;
	bool floating, flying;
	int lastFloatingTime;
};

