#include "GameObject.h"
#include "Game.h"

using namespace std;

CGameObject::CGameObject()
{
	transform = new Transform();
	// DebugOut(L"Base constructor");
	transform->Scale = NormalizedVector();
	transform->Rotation = 0.0f;
	// Step 1: Call event: Awake
	this->Awake();

	// Step 2: Call event: Start (after Awake)
	this->Start();

	// Step 3: Enable object update & render
	this->enabled = true;
}

CGameObject::CGameObject(Vector2 position, Vector2 scale, float rotation)
{
	transform = new Transform();
	this->sprite = sprite;
	this->currentState = "";
	this->transform->Position = position;
	this->transform->Scale = scale;
	this->transform->Rotation = rotation;
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
	if (transform != nullptr) delete transform;
	if (prevTransform != nullptr) delete prevTransform;
	if (deltaTransform != nullptr) delete deltaTransform;
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

void CGameObject::OnEnabled()
{
}

void CGameObject::BeginUpdate()
{
	if (prevTransform == nullptr) prevTransform = new Transform();
	prevTransform->Position = transform->Position;
	prevTransform->Scale = transform->Scale;
	prevTransform->Rotation = transform->Rotation;
}

void CGameObject::PhysicsUpdate(std::vector<GameObject>* objects)
{
	for (auto collider : *colliders)
		collider->PhysicsUpdate(objects);
}

void CGameObject::Update()
{
}

void CGameObject::LateUpdate()
{
}

void CGameObject::EndUpdate()
{
	if (deltaTransform == nullptr) deltaTransform = new Transform();
	deltaTransform->Position = transform->Position - prevTransform->Position;
	deltaTransform->Scale = transform->Scale - prevTransform->Scale;
	deltaTransform->Rotation = transform->Rotation - prevTransform->Rotation;
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
		animations.at(lastState)->SetRelativePosition(transform->Position + visualRelativePosition);
	animations.at(currentState)->SetRelativePosition(transform->Position + visualRelativePosition);
	animations.at(currentState)->SetScale(transform->Scale);
	animations.at(currentState)->SetRotation(transform->Rotation);

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

void CGameObject::OnOverlapped(Collider2D* selfCollider, Collider2D* otherCollider)
{
}

void CGameObject::OnSolidOverlappedExit()
{
}

void CGameObject::OnOffScreen()
{
}

void CGameObject::Initialize()
{
	destroyed = false;
	offscreen = false;
	alwaysUpdate = false;
	
	transform = new Transform();
	rigidbody = new Rigidbody2D();
	colliders = new vector<Collider2D*>();
	effector = Effector2D::Full;
	visualRelativePosition = VectorZero();
	transform->Scale = Vector2(1, 1);

	// Step 1: Call event: Awake
	this->Awake();

	// Step 2: Call event: Start (after Awake)
	this->Start();

	// Step 3: Enable object update & render
	this->enabled = true;
}

Transform& CGameObject::GetTransform()
{
	return *transform;
}

void CGameObject::SetPosition(Vector2 position)
{
	this->transform->Position = position;
}

void CGameObject::SetScale(Vector2 scale)
{
	this->transform->Scale = scale;
}

void CGameObject::SetRotation(float rotation, RotationUnits unit)
{
	transform->Rotation = rotation * (unit == Degree ? PI / 180.0f : 1.0f);
}

int CGameObject::GetRenderOrder()
{
	return this->renderOrder;
}

bool CGameObject::IsOffScreen()
{
	return this->offscreen;
}

void CGameObject::SetOffScreen(bool value)
{
	if (this->offscreen == false && value == true)
		OnOffScreen();
	this->offscreen = value;
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

void CGameObject::SetAlwaysUpdate(bool value)
{
	alwaysUpdate = value;
}

bool CGameObject::IsAlwaysUpdated()
{
	return alwaysUpdate;
}

bool CGameObject::IsEnabled()
{
	return this->enabled;
}

bool CGameObject::IsDestroyed()
{
	return this->destroyed;
}

void CGameObject::SetDestroyed()
{
	destroyed = true;
	enabled = false;
	if (inGrid && cell != nullptr) cell->Remove(this);
}

void CGameObject::SetActive(bool active)
{
	enabled = active;
	if (active) OnEnabled();
	// else if (!global && inGrid && cell != nullptr) cell->Remove(this);
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

Transform& CGameObject::GetDeltaTransform()
{
	return *deltaTransform;
}

Transform& CGameObject::GetPreviousTransform()
{
	return *prevTransform;
}

void CGameObject::SetCell(Cell* cell)
{
	this->cell = cell;
}

Cell* CGameObject::GetCell()
{
	return cell;
}

void CGameObject::SetInGrid(bool value)
{
	inGrid = value;
}

bool CGameObject::GetInGrid()
{
	return inGrid;
}

bool CGameObject::IsGlobal()
{
	return global;
}

bool CGameObject::IsStatic()
{
	return isStatic;
}

void Destroy(GameObject gameObject)
{
	auto activeScene = Game::GetInstance().GetService<SceneManager>()->GetActiveScene();

	if (activeScene != nullptr)
		activeScene->RemoveObject(gameObject);
}
