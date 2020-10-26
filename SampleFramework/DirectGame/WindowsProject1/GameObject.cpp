#include "GameObject.h"
#include "Game.h"

using namespace std;

CGameObject::CGameObject()
{
	// DebugOut(L"Base constructor");
	transform.Scale = NormalizedVector();
	transform.Rotation = 0.0f;
	// Step 1: Call event: Awake
	this->Awake();

	// Step 2: Call event: Start (after Awake)
	this->Start();

	// Step 3: Enable object update & render
	this->enabled = true;
}

CGameObject::CGameObject(Vector2 position, Vector2 scale, float rotation)
{
	this->sprite = sprite;
	this->currentState = "";
	this->transform.Position = position;
	this->transform.Scale = scale;
	this->transform.Rotation = rotation;
	this->enabled = false;
	this->tag = ObjectTags::None;

	// Step 1: Call event: Awake
	this->Awake();

	// Step 2: Call event: Start (after Awake)
	this->Start();

	// Step 3: Enable object update & render
	this->enabled = true;
}

CGameObject::~CGameObject()
{
	auto activeScene = Game::GetInstance().GetService<SceneManager>()->GetActiveScene();
	activeScene->RemoveObject(this);
	if (rigidbody != nullptr) delete rigidbody;
	for (Collider2D* col : *colliders)
		delete col;
	delete colliders;
	OnDestroy();
}

void CGameObject::Awake()
{
}

void CGameObject::Start()
{
}

void CGameObject::PhysicsUpdate(std::vector<GameObject>* objects)
{
	if (GetRigidbody()->IsDynamic() == false) return;

	rigidbody->Update();

	vector<Collider2D*> otherColliders;
	for (auto obj : *objects)
	{
		auto cols = *(obj->GetColliders());
		for (auto col : cols)
			otherColliders.push_back(col);
	}

	for (auto collider : *colliders)
	{
		collider->Update();
		collider->PhysicsUpdate(&otherColliders);
	}
}

void CGameObject::Update()
{
}

void CGameObject::LateUpdate()
{
}

void CGameObject::OnAnimationEnd()
{
}

void CGameObject::PreRender()
{
}

void CGameObject::Render(Vector2 translation)
{
	bool stateExistence = animations.find(currentState) != animations.end();
	if (animations.empty() || stateExistence == false) return;

	if (animations.find(lastState) != animations.end())
		animations.at(lastState)->SetRelativePosition(transform.Position + visualRelativePosition);
	animations.at(currentState)->SetRelativePosition(transform.Position + visualRelativePosition);
	animations.at(currentState)->SetScale(transform.Scale);
	animations.at(currentState)->SetRotation(transform.Rotation);

	animations.at(currentState)->Render(translation);
}

void CGameObject::OnDestroy()
{
}

void CGameObject::OnKeyUp(int keyCode)
{
}

void CGameObject::OnKeyDown(int keyCode)
{
}

void CGameObject::OnCollisionEnter(Collider2D* selfCollider, vector<CollisionEvent*> collisions)
{
}

void CGameObject::OnTriggerEnter(Collider2D* selfCollider, vector<CollisionEvent*> collisions)
{
}

void CGameObject::Initialize()
{
	rigidbody = new Rigidbody2D();
	colliders = new vector<Collider2D*>();
	effector = Effector2D::Full;
	visualRelativePosition = VectorZero();

	// Step 1: Call event: Awake
	this->Awake();

	// Step 2: Call event: Start (after Awake)
	this->Start();

	// Step 3: Enable object update & render
	this->enabled = true;
}

Transform CGameObject::GetTransform()
{
	return this->transform;
}

void CGameObject::SetPosition(Vector2 position)
{
	this->transform.Position = position;
}

void CGameObject::SetScale(Vector2 scale)
{
	this->transform.Scale = scale;
}

void CGameObject::SetRotation(float rotation, RotationUnits unit)
{
	transform.Rotation = rotation * (unit == Degree ? PI / 180.0f : 1.0f);
}

int CGameObject::GetRenderOrder()
{
	return this->renderOrder;
}

void CGameObject::AddAnimation(string stateName, Animation* animation, bool loop)
{
	animation->SetLoop(loop);
	animation->SetGameObject(this);
	animations.insert(make_pair(stateName, animation));
}

void CGameObject::SetState(string state)
{
	if (animations.find(state) != animations.end())
	{
		lastState = currentState;
		currentState = state;
		animations.at(state)->Play(false);
		return;
	}
	DebugOut(L"Couldnt find animation[]\n");
}

Animation* CGameObject::GetState(std::string state)
{
	if (animations.find(state) != animations.end())
		return animations.at(state);
	return nullptr;
}

Rigidbody2D* CGameObject::GetRigidbody()
{
	return this->rigidbody;
}

std::vector<Collider2D*>* CGameObject::GetColliders()
{
	return this->colliders;
}

void CGameObject::SetVisualRelativePosition(Vector2 position)
{
	this->visualRelativePosition = position;
}

Vector2 CGameObject::GetVisualRelativePosition()
{
	return this->visualRelativePosition;
}

bool CGameObject::IsEnabled()
{
	return this->enabled;
}

void CGameObject::SetActive(bool active)
{
	enabled = active;
}

void CGameObject::SetTag(ObjectTags tag)
{
	this->tag = tag;
}

ObjectTags CGameObject::GetTag()
{
	return this->tag;
}

void CGameObject::SetEffector(Effector2D effector)
{
	this->effector = effector;
}

Effector2D CGameObject::GetEffector()
{
	return this->effector;
}

void Destroy(GameObject gameObject)
{
	auto activeScene = Game::GetInstance().GetService<SceneManager>()->GetActiveScene();

	if (activeScene != nullptr)
		activeScene->RemoveObject(gameObject);

	delete gameObject;
}
