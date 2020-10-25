#pragma once
#include "Sprite.h"
#include <d3d9.h>
#include <d3dx9.h>
#include <vector>
#include "AnimationFrame.h"
#include "Transform.h"
#include <functional>
#include "GameObject.h"

using namespace std;

class CGameObject;
typedef CGameObject* GameObject;
class Animation
{
public:
	Animation(std::string id, DWORD defaultFrameTime)
	{
		this->id = id;
		currentFrame = -1; this->defaultFrameTime = defaultFrameTime;
		lastFrameTime = 0;
		transform.Scale = NormalizedVector();
		transform.Rotation = 0.0f;
		ResetSpeedMultiplier();
	}
	Animation(const Animation& obj)
	{
		this->id = obj.id;
		this->currentFrame = obj.currentFrame;
		this->defaultFrameTime = obj.defaultFrameTime;
		lastFrameTime = obj.lastFrameTime;
		std::vector<AnimFrame> newFrames(obj.frames);
		this->frames = newFrames;
		transform = CreateTransform(obj.transform.Position, obj.transform.Scale, obj.transform.Rotation);
		ResetSpeedMultiplier();
	}
	void AddFrame(Sprite sprite, D3DXVECTOR2 pos, DWORD frameTime = 0);
	void Render(Vector2 translation);
	void SetRelativePosition(D3DXVECTOR2 pos)	{ transform.Position = pos;			}
	Vector2 GetRelativePosition()				{ return transform.Position;		}

	void SetScale(Vector2 scale)				{ transform.Scale = scale;			}
	Vector2 GetScale()							{ return transform.Scale;			}

	void SetRotation(float rotation)			{ transform.Rotation = rotation;	}
	float GetRotation()							{ return transform.Rotation;		}

	void SetSpeedMultiplier(float speed);
	void ResetSpeedMultiplier()					{ this->speedMultiplier = 1.0f;			}
	float GetSpeedMultiplier()					{ return speedMultiplier;			}

	void SetLoop(bool loop);
	bool GetLoop();

	void Play(bool continueFromLastState = true);

	void SetGameObject(GameObject go);

protected:
	float					speedMultiplier;
	std::vector<AnimFrame>	frames;
	DWORD					defaultFrameTime, lastFrameTime;
	int						currentFrame;
	std::string				id;
	bool					loop, playing;

	Transform				transform;
	GameObject				gameObject;

};
typedef Animation* PAnimation;