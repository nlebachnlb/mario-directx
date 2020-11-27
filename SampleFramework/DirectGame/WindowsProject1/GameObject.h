#pragma once
#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <d3d9.h>
#include <d3dx9.h>
#include "Sprite.h"
#include <vector>
#include "Animation.h"
#include "Transform.h"
#include "Utils.h"
#include "Rigidbody2D.h"
#include "Collider2D.h"
#include "ObjectTags.h"
#include "Effector2D.h"

class Collider2D;
struct CollisionEvent;
typedef CollisionEvent* PCollisionEvent;

class Animation;
typedef Animation* PAnimation;

class CGameObject
{
public:
	CGameObject();
	CGameObject(Vector2 position, Vector2 scale, float rotation);
	virtual ~CGameObject();

#pragma region Events
	// Awake: Call at very first in Initialization, use to initialize data
	virtual void Awake();
	// Start: Call next to Awake, use to set default value for data
	virtual void Start();
	// OnEnabled: Call when SetActive(true) is called
	virtual void OnEnabled();
	// PhysicsUpdate (auto): update physic collisions, call automatically, DO NOT override
	virtual void PhysicsUpdate(std::vector<CGameObject*>* objects);
	// Update: Main update, OVERRIDE for purposes
	virtual void Update();
	// Late Update: Call after Update, use for post processing data [OVERRIDE]
	virtual void LateUpdate();
	// OnAnimationEnd: Call at the end of a non-looped animation [OVERRIDE]
	virtual void OnAnimationEnd();
	// PreRender: Call before Render (same block of Render), initialize render information [OVERRIDE for purposes]
	virtual void PreRender();
	// Render: Render the graphics based on initialized data
	virtual void Render(Vector2 translation);
	// OnDestroy: Call before destroying an object [OVERRIDE for purposes]
	virtual void OnDestroy();
	// OnKeyUp, OnKeyDown: Response event of Keyboard [OVERRIDE for purposes]
	virtual void OnKeyUp(int keyCode);
	virtual void OnKeyDown(int keyCode);
	// OnCollisionEnter: Called when collision occured in PhysicUpdate
	virtual void OnCollisionEnter(Collider2D* selfCollider, std::vector<CollisionEvent*> collisions);
	virtual void OnTriggerEnter(Collider2D* selfCollider, std::vector<CollisionEvent*> collisions);
	// OnOverlapped: Called when an intersection of selfBox with another box
	virtual void OnOverlapped(Collider2D* selfCollider, Collider2D* otherCollider);
	virtual void OnSolidOverlappedExit();
	// OnOffScreen: Called when an object is out of the camera view
	virtual void OnOffScreen();
#pragma endregion

	virtual void Initialize();

	Transform GetTransform();
	void SetPosition(Vector2 position);
	void SetScale(Vector2 scale);
	void SetRotation(float rotation, RotationUnits unit = Degree);
	int  GetRenderOrder();
	bool IsOffScreen();
	void SetOffScreen(bool value);

	void AddAnimation(std::string stateName, Animation* animation, bool loop = true);
	void SetState(std::string state);
	Animation* GetState(std::string state);

	Rigidbody2D* GetRigidbody();
	std::vector<Collider2D*>* GetColliders();

	void SetVisualRelativePosition(Vector2 position);
	Vector2 GetVisualRelativePosition();

	bool IsEnabled();
	bool IsDestroyed();
	void SetDestroyed();
	void SetActive(bool active);

	void SetTag(ObjectTags tag);
	ObjectTags GetTag();

	void SetEffector(Effector2D effector);
	Effector2D GetEffector();

protected:
	Transform transform;
	Sprite sprite;
	Vector2 visualRelativePosition;

	int id;
	std::string currentState, lastState;
	std::unordered_map<std::string, Animation*> animations;

	int renderOrder;
	bool enabled, destroyed, offscreen;
	ObjectTags tag;
	Effector2D effector;

	Rigidbody2D* rigidbody;
	std::vector<Collider2D*>* colliders;

private:
	std::vector<Collider2D*> beingUpdatedColliders;
};

typedef CGameObject* GameObject;

template<typename T>
T* Instantiate();

void Destroy(GameObject gameObject);

#endif

template<typename T>
inline T* Instantiate()
{
	T* obj = new T();
	obj->Initialize();
	return obj;
}
