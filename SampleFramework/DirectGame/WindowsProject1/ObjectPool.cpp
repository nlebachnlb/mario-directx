#include "ObjectPool.h"

void ObjectPool::Add(GameObject object)
{
    pool.push_back(object);
    object->SetActive(false);
}

GameObject ObjectPool::Instantiate()
{
    if (IsEmpty())
        return nullptr;
    auto obj = pool.at(0);
    obj->SetActive(true);
    pool.erase(pool.begin());
    return obj;
}

void ObjectPool::Revoke(GameObject obj)
{
    obj->SetActive(false);
    if (std::find(pool.begin(), pool.end(), obj) != pool.end()) return;
    pool.push_back(obj);
}

bool ObjectPool::IsEmpty()
{
    return pool.empty();
}

void ObjectPool::RegisterPoolToScene(Scene* scene)
{
    // DebugOut(L"[add fireball]");
    for (auto x : pool)
        scene->AddObject(x);
}
