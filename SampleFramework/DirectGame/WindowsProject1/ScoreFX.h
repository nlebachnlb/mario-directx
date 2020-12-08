#pragma once
#include "FXObject.h"
enum class Score
{
	S100, S200, S400, S800,
	S1000, S2000, S4000, S8000,
	S1UP
};

const float FLOATING_SPEED = 0.5f;
const float DRAG = 0.02f;

class ScoreFX : public FXObject
{
public:
	void Awake() override;
	void Start() override;
	void PreRender() override;
	void Render(Vector2 translation) override;

	void SetLevel(Score level);
private:
	int level;
	Sprite scoreLevel[9];
	float speed;
};

