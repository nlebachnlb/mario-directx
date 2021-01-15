#include "RedKoopaParatroopa.h"
#include "AnimationDatabase.h"
#include "Game.h"
#include "Mathf.h"

void RedKoopaParatroopa::Start()
{
	SetState("Fly");
	troopa = Troopa::Red;
	lifeTime = 0;
	rigidbody->SetGravity(0);
}

void RedKoopaParatroopa::Movement()
{
	auto dt = Game::DeltaTime() * Game::GetTimeScale();
	lifeTime += dt;
	auto a = amplitude;
	auto t = roundtrip;
	auto omega = 2 * Mathf::Pi / (float)t;
	auto phix = Mathf::Pi;
	auto phiy = Mathf::Pi / 2.0f;
	auto vx = -omega * a.x * Mathf::Sin(omega * lifeTime + phix);
	auto vy = +omega * a.y * Mathf::Cos(omega * lifeTime + phiy);
	rigidbody->SetVelocity(&Vector2(vx, vy));
	// transform.Position = startPosition + Vector2(x, y);
}

void RedKoopaParatroopa::InitAnimations()
{
	auto animations = Game::GetInstance().GetService<AnimationDatabase>();
	AddAnimation("Fly", animations->Get("ani-red-koopa-paratroopa-fly"));
	AddAnimation("Idle", animations->Get("ani-red-koopa-paratroopa-idle"));
	AddAnimation("Die", animations->Get("ani-red-koopa-troopa-shell-idle"));
}

void RedKoopaParatroopa::SetAmplitude(Vector2 value)
{
	amplitude = value;
}

void RedKoopaParatroopa::SetRoundtripTime(int time)
{
	roundtrip = time;
}

void RedKoopaParatroopa::SetStartPosition(Vector2 value)
{
	startPosition = value;
}
