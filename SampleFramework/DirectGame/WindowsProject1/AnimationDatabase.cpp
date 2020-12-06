#include "AnimationDatabase.h"
#include "tinyxml.h"
#include "Consts.h"
#include "Game.h"

void AnimationDatabase::Initialization()
{
	auto root = Game::GetInstance();
	LoadAnimationFile(root.GetSourcePathOf(CATEGORY_ANIMATION, DB_ANIMATION_MARIO));
	LoadAnimationFile(root.GetSourcePathOf(CATEGORY_ANIMATION, DB_ANIMATION_ENEMY));
	LoadAnimationFile(root.GetSourcePathOf(CATEGORY_ANIMATION, DB_ANIMATION_MISC));
	LoadAnimationFile(root.GetSourcePathOf(CATEGORY_ANIMATION, DB_ANIMATION_EFFECT));
	LoadAnimationFile(root.GetSourcePathOf(CATEGORY_ANIMATION, DB_ANIMATION_HUD));
}

bool AnimationDatabase::LoadAnimationFile(string path)
{
	OutputDebugStringW(ToLPCWSTR(path.c_str()));
	TiXmlDocument document(path.c_str());
	if (!document.LoadFile())
	{
		OutputDebugStringW(ToLPCWSTR(document.ErrorDesc()));
		return false;
	} 

	TiXmlElement* root = document.RootElement();
	for (TiXmlElement* info = root->FirstChildElement("Textures"); info != nullptr; info = info->NextSiblingElement("Textures"))
	{
		string gameObjectID = info->Attribute("gameObjectId");
		string textureID = info->Attribute("textureId");

		OutputDebugStringW(ToLPCWSTR("Gameobject id: " + gameObjectID + '\n'));
		OutputDebugStringW(ToLPCWSTR("Texture id: " + textureID + '\n'));

		for (TiXmlElement* node = info->FirstChildElement("Animation"); node != nullptr; node = node->NextSiblingElement("Animation"))
		{
			string aniId = node->Attribute("aniId");
			float frameTime;
			node->QueryFloatAttribute("frameTime", &frameTime);
			// string name = node->Attribute("name");
			string name = "";

			// OutputDebugStringW(ToLPCWSTR(aniId + ':' + to_string(frameTime) + ':' + name + '\n'));
			Animation* animation = new Animation(aniId, frameTime);

			// Sprite ref
			for (TiXmlElement* sprNode = node->FirstChildElement("Sprite"); sprNode != nullptr; sprNode = sprNode->NextSiblingElement("Sprite"))
			{
				string id = sprNode->Attribute("id");
				Sprite sprite = Game::GetInstance().GetService<SpriteManager>()->Get(id);
				float detailFrameTime;
				if (sprNode->QueryFloatAttribute("frameTime", &detailFrameTime) != TIXML_SUCCESS)
					detailFrameTime = frameTime;
				animation->AddFrame(sprite, VectorZero(), detailFrameTime * 0.5f);

				// OutputDebugStringW(ToLPCWSTR("|--" + id + ':' + to_string(detailFrameTime * 0.5f) + '\n'));
			}

			Add(aniId, animation);
		}
	}
}

void AnimationDatabase::Add(string id, Animation* animation)
{
	database.insert(make_pair(id, animation));
}

Animation* AnimationDatabase::Get(string id)
{
	return database.at(id);
}

Animation* AnimationDatabase::Clone(string id)
{
	Animation* clone = new Animation(*Get(id));
	return clone;
}
