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
    if (manual)
    {
        mapData = MapData::FromTMX(filePath);

        if (spawnerManager == nullptr) spawnerManager = new SpawnerManager();
        spawnerManager->ClearServices();

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
                    QuestionBlock* solid = Instantiate<QuestionBlock>();
                    solid->SetPosition(position);
                    this->gameObjects.push_back(solid);
                }
            }

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
                            auto goomba = Instantiate<Goomba>();
                            goomba->SetPosition(position);
                            goomba->SetPool(goombaSpawner->GetPool());
                            goombaSpawner->AddPrototype(oid, new SpawnPrototype(position, goomba));
                            this->gameObjects.push_back(goomba);
                        }
                    }
                    /*else if (name.compare("koopa-shell") == 0)
                    {
                        auto koopaSpawner = spawnerManager->GetService<KoopaSpawner>();
                        if (koopaSpawner == nullptr)
                        {
                            koopaSpawner = new KoopaSpawner();
                            spawnerManager->AddService(koopaSpawner);
                        }

                        auto oid = objects->at(i)->id;

                        KoopasShell* shell = nullptr;
                        if (type.compare("red") == 0)
                        {
                            shell = koopaSpawner->InstantiateShell(position);
                            this->gameObjects.push_back(shell);
                        }
                    }*/
                    else if (name.compare("koopa") == 0)
                    {
                        auto koopaSpawner = spawnerManager->GetService<KoopaSpawner>();
                        if (koopaSpawner == nullptr)
                        {
                            koopaSpawner = new KoopaSpawner();
                            spawnerManager->AddService(koopaSpawner); 
                        }

                        if (type.compare("red") == 0)
                        {
                            auto oid = objects->at(i)->id;
                            auto koopa = Instantiate<RedKoopa>();
                            koopa->SetPosition(position);
                            koopa->SetPool(koopaSpawner->GetPool());
                            koopaSpawner->AddPrototype(oid, new SpawnPrototype(position, koopa));
                            this->gameObjects.push_back(koopa);
                        }
                    }
                }
            }
        }
        return;
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
