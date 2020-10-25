#pragma once
#include <unordered_map>
#include "Scene.h"
#include "Service.h"

class SceneManager : public Service
{
public:
	void Initialization();
	bool LoadScene(Scene* scene);
	void UnloadScene(std::string id);
	std::string GetActiveSceneID() { return activeSceneID; }
	Scene* GetActiveScene();
	Scene* Get(std::string id) { return loadedScenes.at(id); }
private:
	unordered_map<std::string, Scene*> loadedScenes;
	std::string activeSceneID;
};

