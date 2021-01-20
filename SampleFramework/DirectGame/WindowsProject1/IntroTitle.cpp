#include "IntroTitle.h"
#include "Game.h"
#include "Mathf.h"

void IntroTitle::Awake()
{
	alwaysUpdate = true;
	auto spr = Game::GetInstance().GetService<SpriteManager>();
	sprites[0] = spr->Get("spr-big-cloud-0");
	sprites[1] = spr->Get("spr-small-cloud-0");
	sprites[2] = spr->Get("spr-title-0");

	renderOrder = 1;
	version = nullptr;
}

void IntroTitle::Start()
{
	animStep = 0;
	timer = 0;
	SetPosition(Vector2(412, -300));
}

void IntroTitle::Update()
{
	auto dt = Game::DeltaTime() * Game::GetTimeScale();
	switch (animStep)
	{
	case 0:
	{
		auto pos = transform->Position;
		//DebugOut(L"Position: %f, %f\n", pos.x, pos.y);
		auto distance = Mathf::Magnitude(DESTINATION - pos);

		if (distance < 3)
		{
			SetPosition(DESTINATION);
			animStep = 1;
		}
		else
		{
			auto speed = Mathf::Magnitude(DESTINATION - Vector2(412, -300)) / MOVEMENT_DURATION;
			pos += Vector2(0, speed * dt);
			SetPosition(pos);
		}
	}
	break;
	case 1:
	{
		// Shaking fx
		timer += dt;
		if (timer > 500)
		{
			timer = 0;
			animStep = 2;
			SetPosition(DESTINATION);
		}
		else
		{
			transform->Position.y = DESTINATION.y + 5 * Mathf::Sin((2 * Mathf::Pi / 40.0f) * timer);
		}
	}
	break;
	}

	if (version != nullptr)
	{
		version->SetPosition(transform->Position + Vector2(14, 156));
	}
}

void IntroTitle::Render(Vector2 translation)
{
	// Draw title
	auto titlePos = transform->Position + translation;
	sprites[2]->Draw(titlePos.x, titlePos.y);

	auto cloudPos0 = titlePos + Vector2(-364, -58);
	sprites[0]->Draw(cloudPos0.x, cloudPos0.y, 0, 0);

	auto cloudPos1 = titlePos + Vector2(248, -104);
	sprites[0]->Draw(cloudPos1.x, cloudPos1.y, 0, 0);

	auto cloudPos2 = titlePos + Vector2(-388, 62);
	sprites[1]->Draw(cloudPos2.x, cloudPos2.y, 0, 0);
}

void IntroTitle::SetVersion(GameObject version)
{
	this->version = version;
}
