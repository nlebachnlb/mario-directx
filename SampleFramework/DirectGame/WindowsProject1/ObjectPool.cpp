#include "ObjectPool.h"

void ObjectPool::Add(GameObject object)
{
    pool.push(object);
    object->SetActive(false);
}

GameObject ObjectPool::Instantiate()
{
    if (IsEmpty())
        return nullptr;
    auto obj = pool.front();
    obj->SetActive(true);
    pool.pop();
    return obj;
}

void ObjectPool::Revoke(GameObject obj)
{
    obj->SetActive(false);
    pool.push(obj);
}

bool ObjectPool::IsEmpty()
{
    return pool.empty();
}
