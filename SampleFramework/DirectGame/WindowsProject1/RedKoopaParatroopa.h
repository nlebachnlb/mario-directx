#pragma once
#include "KoopaParatroopa.h"
class RedKoopaParatroopa : public KoopaParatroopa
{
public:
	void Start() override;
	void Movement() override;
	void InitAnimations() override;

	void SetAmplitude(Vector2 value);
	void SetRoundtripTime(int time);
	void SetStartPosition(Vector2 value);

protected:
	Vector2 amplitude, startPosition;
	int roundtrip;
	long long lifeTime;
};

