#include "SceneManager.h"
#include "Utils.h"

void SceneManager::Initialization()
{
}

bool SceneManager::LoadScene(Scene* scene)
{
    UnloadCurrentScene();
    loadRequests.push_back(scene);
    DebugOut(L"Loading scene");
    
    return true;
}

void SceneManager::UnloadScene(std::string id)
{
    if (loadedScenes.find(id) != loadedScenes.end())
    {
        unloadRequests.push_back(id);
    }
}

void SceneManager::UnloadCurrentScene()
{
    UnloadScene(activeSceneID);
}

void SceneManager::ProcessLoadRequests()
{
    if (loadRequests.size() > 0)
    {
        for (auto scene : loadRequests)
        {
            scene->Load();
            // loadedScenes.insert(make_pair(scene->GetID(), scene));
            loadedScenes[scene->GetID()] = scene;
            activeSceneID = scene->GetID();
            scene->Init();
        }

        loadRequests.clear();
    }
}

void SceneManager::ProcessUnloadRequests()
{
    if (unloadRequests.size() > 0)
    {
        for (auto req : unloadRequests)
        {
            auto scene = loadedScenes.at(req);
            loadedScenes.erase(req);
            scene->Unload();
            // loadedScenes.at(id) = nullptr;
            delete scene;
            activeSceneID = "";
        }

        unloadRequests.clear();
    }
}

Scene* SceneManager::GetActiveScene()
{
    if (activeSceneID == "") return nullptr;
    if (loadedScenes.find(activeSceneID) != loadedScenes.end())
        return loadedScenes.at(activeSceneID);
    else
        return nullptr;
}
