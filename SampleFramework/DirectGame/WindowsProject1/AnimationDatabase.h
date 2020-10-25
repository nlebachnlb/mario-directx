#pragma once
#include "Service.h"
#include "Animation.h"
#include "Sprite.h"

//class AnimationInfo
//{
//public:
//	std::string id;
//	int frameTime;
//	std::string name;
//	std::unordered_map<
//};

class AnimationDatabase : public Service
{
public:
	void Initialization();
	bool LoadAnimationFile(string path);
	void Add(string id, Animation* animation);
	Animation* Get(string id);
	Animation* Clone(string id);

private:
	std::unordered_map<string, Animation*> database;
};

