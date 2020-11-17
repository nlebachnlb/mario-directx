#include "Animation.h"
#include "Game.h"

void Animation::AddFrame(Sprite sprite, D3DXVECTOR2 pos, DWORD frameTime)
{
	if (frameTime == 0) frameTime = defaultFrameTime;
	AnimFrame frame = new AnimationFrame(sprite, frameTime, pos);
	frames.push_back(frame);
}

void Animation::Render(Vector2 translation)
{
	if ((frames.size() == 0 || speedMultiplier == 0.0f) == false)
	{
		DWORD currentTime = GetTickCount();

		if (currentFrame == -1)
		{
			currentFrame = 0;
			lastFrameTime = currentTime;
		}
		else
		{
			if (currentTime - lastFrameTime > frames[currentFrame]->GetTime() * Game::GetTimeScale() / speedMultiplier)
			{
				if (currentFrame == frames.size() - 1 && loop == false)
				{
					gameObject->OnAnimationEnd();
					playing = false;
				}
				else if (playing)
				{
					currentFrame = (currentFrame + 1) % frames.size();
					lastFrameTime = currentTime;
				}
			}
		}

		Vector2 pos = transform.Position + translation;
		frames.at(currentFrame)->GetSprite()->Draw(pos.x, pos.y, transform.Scale, transform.Rotation, alpha);
	}
}

void Animation::SetSpeedMultiplier(float speed)
{
	this->speedMultiplier = speed;
}

void Animation::SetLoop(bool loop)
{
	this->loop = loop;
}

bool Animation::GetLoop()
{
	return this->loop;
}

void Animation::Play(bool continueFromLastState)
{
	if (continueFromLastState == false && !playing)
	{
		lastFrameTime = GetTickCount();
		currentFrame = -1;
	}
	playing = true;
}

void Animation::SetGameObject(GameObject go)
{
	this->gameObject = go;
}

int Animation::GetAlpha()
{
	return this->alpha;
}

void Animation::SetAlpha(int alpha)
{
	this->alpha = alpha;
}
