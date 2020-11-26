#include "GameMap.h"
#include "Game.h"
#include "Utils.h"
#include "SolidBox.h"
#include "GhostPlatform.h"
#include "Goomba.h"
#include "GoombaSpawner.h"
#include "RedKoopasShell.h"
#include "KoopaSpawner.h"
#include "RedKoopa.h"
#include "QuestionBlock.h"
#include "EffectPool.h"
#include "Brick.h"
#include "Coin.h"
#include "GreenKoopa.h"
#include "PlantSpawner.h"

GameMap::GameMap()
{
    mapData = nullptr;
    spawnerManager = nullptr;
}

GameMap::GameMap(std::string filePath)
{
	this->filePath = filePath;
}

void GameMap::Load(std::string filePath, bool manual)
{
    mapData = MapData::FromTMX(filePath);

    if (spawnerManager == nullptr) spawnerManager = new SpawnerManager();
    spawnerManager->ClearServices();

    auto fxPool = new EffectPool();
    spawnerManager->AddService(fxPool);

    auto tilesets = mapData->GetTilesets();
    for (auto x : *tilesets)
    {
        auto tileset = x.second;
        auto texManager = Game::GetInstance().GetService<TextureManager>();
        auto tilesetTexture = texManager->LoadTexture(ToLPCWSTR(tileset->GetSource()));

        Tile tile = new CTile(
            "map-tileset",
            0, 0,
            tileset->GetImageWidth(), tileset->GetImageHeight(),
            tilesetTexture);

        this->tilesets.insert(make_pair(tileset->GetID(), tile));
    }

    auto objectGroups = mapData->GetObjectGroups();
    // Load game objects
    gameObjects.clear();
    for (auto x : *objectGroups)
    {
        auto objectGroup = x.second;
        auto objects = objectGroup->GetObjects();
        auto groupName = objectGroup->GetName();

        if (groupName.compare("Solid") == 0)
        {
            for (int i = 0; i < objects->size(); ++i)
            {
                Vector2 position(objects->at(i)->x, objects->at(i)->y);
                Vector2 size(objects->at(i)->width, objects->at(i)->height);
                DebugOut(L"Solid: %f, %f, %f, %f\n", position.x, position.y, size.x, size.y);

                SolidBox* solid = Instantiate<SolidBox>();
                solid->SetPosition(position + (size / 2.0f));
                solid->GetColliders()->at(0)->SetBoxSize(size);
                this->gameObjects.push_back(solid);

                auto boxSize = solid->GetColliders()->at(0)->GetBoxSize();
                // DebugOut(L"BoxSize: %f,%f,%f,%f\n", solid->GetTransform().Position.x, solid->GetTransform().Position.y, boxSize.x, boxSize.y);
            }
        }

        if (groupName.compare("Ghost") == 0)
        {
            for (int i = 0; i < objects->size(); ++i)
            {
                Vector2 position(objects->at(i)->x, objects->at(i)->y);
                Vector2 size(objects->at(i)->width, objects->at(i)->height);

                GhostPlatform* solid = Instantiate<GhostPlatform>();
                solid->SetPosition(position + (size / 2.0f));
                solid->GetColliders()->at(0)->SetBoxSize(size);
                this->gameObjects.push_back(solid);

                auto boxSize = solid->GetColliders()->at(0)->GetBoxSize();
                // DebugOut(L"BoxSize: %f,%f,%f,%f\n", solid->GetTransform().Position.x, solid->GetTransform().Position.y, boxSize.x, boxSize.y);
            }
        }

        if (groupName.compare("QuestionBlocks") == 0)
        {
            for (int i = 0; i < objects->size(); ++i)
            {
                Vector2 position(objects->at(i)->x, objects->at(i)->y);
                auto name = objects->at(i)->name;
                auto type = stoi(objects->at(i)->type);

                QuestionBlock* solid = Instantiate<QuestionBlock>();
                solid->SetPosition(position);
                this->gameObjects.push_back(solid);

                if (name.compare("bcoin") == 0)
                    solid->SetItem({ ItemTags::Coin, type });
                else if (name.compare("bmushroom") == 0)
                    solid->SetItem({ ItemTags::Mushroom, type });
                else if (name.compare("bleaf") == 0)
                    solid->SetItem({ ItemTags::Leaf, type });
            }
        }

        if (groupName.compare("Bricks") == 0)
        {
            for (int i = 0; i < objects->size(); ++i)
            {
                Vector2 position(objects->at(i)->x, objects->at(i)->y);
                Brick* solid = Instantiate<Brick>();
                solid->SetPosition(position);
                this->gameObjects.push_back(solid);
            }
        }

        if (groupName.compare("Items") == 0)
        {
            for (int i = 0; i < objects->size(); ++i)
            {
                auto name = objects->at(i)->name;
                Vector2 position(objects->at(i)->x, objects->at(i)->y);
                auto type = objects->at(i)->type;

                if (name.compare("coin") == 0)
                {
                    if (type.compare("basic") == 0)
                    {
                        auto oid = objects->at(i)->id;
                        auto coin = Instantiate<Coin>();
                        coin->SetPosition(position);
                        this->gameObjects.push_back(coin);
                    }
                }
            }
        }
    }
    return;
}

void GameMap::LoadEnemy()
{
    auto objectGroups = mapData->GetObjectGroups();
    for (auto x : *objectGroups)
    {
        auto objectGroup = x.second;
        auto objects = objectGroup->GetObjects();
        auto groupName = objectGroup->GetName();

        if (groupName.compare("Enemies") == 0)
        {
            for (int i = 0; i < objects->size(); ++i)
            {
                auto name = objects->at(i)->name;
                Vector2 position(objects->at(i)->x, objects->at(i)->y);
                auto type = objects->at(i)->type;

                if (name.compare("goomba") == 0)
                {
                    auto goombaSpawner = spawnerManager->GetService<GoombaSpawner>();
                    if (goombaSpawner == nullptr)
                    {
                        goombaSpawner = new GoombaSpawner();
                        spawnerManager->AddService(goombaSpawner);
                    }

                    if (type.compare("basic") == 0)
                    {
                        auto oid = objects->at(i)->id;
                        /*auto goomba = Instantiate<Goomba>();
                        goomba->SetPosition(position);
                        goomba->SetPool(goombaSpawner->GetPool());
                        goombaSpawner->AddPrototype(oid, new SpawnPrototype(position, goomba));
                        this->gameObjects.push_back(goomba);*/
                        goombaSpawner->Spawn("enm-tan-goomba", position);
                    }
                    else if (type.compare("red-para") == 0)
                        goombaSpawner->Spawn("enm-red-para-goomba", position);
                }
                else if (name.compare("koopa") == 0)
                {
                    auto koopaSpawner = spawnerManager->GetService<KoopaSpawner>();
                    if (koopaSpawner == nullptr)
                    {
                        koopaSpawner = new KoopaSpawner();
                        spawnerManager->AddService(koopaSpawner);
                    }

                    AbstractEnemy* koopa = nullptr;
                    if (type.compare("red") == 0)
                        koopa = koopaSpawner->Spawn("enm-red-koopa", position);
                    else if (type.compare("green") == 0)
                        koopa = koopaSpawner->Spawn("enm-green-koopa", position);
                    else if (type.compare("green-para") == 0)
                        koopa = koopaSpawner->Spawn("enm-green-para-koopa", position);

                    if (koopa != nullptr)
                    {
                        auto oid = objects->at(i)->id;
                        /*koopa->SetPosition(position);
                        koopa->SetPool(koopaSpawner->GetPool());
                        koopaSpawner->AddPrototype(oid, new SpawnPrototype(position, koopa));
                        this->gameObjects.push_back(koopa);*/
                    }
                }
                else if (name.compare("piranha-plant") == 0)
                {
                    auto plantSpawner = spawnerManager->GetService<PlantSpawner>();
                    if (plantSpawner == nullptr)
                    {
                        plantSpawner = new PlantSpawner();
                        spawnerManager->AddService(plantSpawner);
                    }

                    if (type.compare("green") == 0)
                        plantSpawner->Spawn("enm-green-piranha-plant", position);
                }
            }
        }
    }
}

void GameMap::Initialization()
{
    
}

MapData* GameMap::GetMapData()
{
    return this->mapData;
}

std::vector<GameObject> GameMap::GetGameObjects()
{
    return this->gameObjects;
}

Tile GameMap::GetTileset(int id)
{
    return tilesets.at(id);
}

int GameMap::GetMapWidth()
{
    return this->mapData->GetMapWidthInPixels();
}

int GameMap::GetMapHeight()
{
    return this->mapData->GetMapHeightInPixels();
}

void GameMap::Update()
{
}

void GameMap::Render()
{
    
}

SpawnerManager* GameMap::GetSpawnerManager()
{
    return this->spawnerManager;
}

GameMap::~GameMap()
{
    if (mapData != nullptr) delete mapData;
    spawnerManager->ClearServices();
    delete spawnerManager;
}
