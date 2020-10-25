#include "SceneManager.h"
#include "Utils.h"

void SceneManager::Initialization()
{
}

bool SceneManager::LoadScene(Scene* scene)
{
    DebugOut(L"Loading scene");
    scene->Load();
    loadedScenes.insert(make_pair(scene->GetID(), scene));
    activeSceneID = scene->GetID();
    
    return true;
}

void SceneManager::UnloadScene(std::string id)
{
    if (loadedScenes.find(id) != loadedScenes.end()) 
        loadedScenes.at(id)->Unload();
}

Scene* SceneManager::GetActiveScene()
{
    if (activeSceneID == "") return nullptr;
    if (loadedScenes.find(activeSceneID) != loadedScenes.end())
        return loadedScenes.at(activeSceneID);
    else
        return nullptr;
}
