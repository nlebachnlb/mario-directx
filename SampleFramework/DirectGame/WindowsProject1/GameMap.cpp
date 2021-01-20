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
#include "Pipe.h"
#include "VenusFireTrap.h"
#include "WarpEntrance.h"
#include "WarpMark.h"
#include "ItemBrick.h"
#include "LevelReward.h"
#include "MapTree.h"
#include "MapNode.h"
#include "Gate.h"
#include "Graph.h"
#include "WorldMapScene.h"
#include "GravityPlatform.h"
#include "RedKoopaParatroopa.h"

GameMap::GameMap()
{
    mapData = nullptr;
    spawnerManager = nullptr;
}

GameMap::GameMap(std::string filePath)
{
	this->filePath = filePath;
}

void GameMap::LoadMapData(std::string filePath, std::string metaPath)
{
    mapData = MapData::FromTMX(filePath);

    // Integrate grid data
    if (!metaPath.empty()) mapData->IntegrateGridData(metaPath);
}

void GameMap::Load()
{
    if (spawnerManager == nullptr) spawnerManager = new SpawnerManager();
    spawnerManager->ClearServices();

    auto fxPool = new EffectPool();
    spawnerManager->AddService(fxPool);
    DebugOut(L"Add FX Pool\n");

    auto tilesets = mapData->GetTilesets();
    auto texManager = Game::GetInstance().GetService<TextureManager>();
    for (auto x : *tilesets)
    {
        auto tileset = x.second;
        auto tilesetTexture = texManager->LoadTexture(ToLPCWSTR(tileset->GetSource()));
        textures.push_back(tilesetTexture);

        Tile tile = new CTile(
            "map-tileset",
            0, 0,
            tileset->GetImageWidth(), tileset->GetImageHeight(),
            tilesetTexture);

        this->tilesets.insert(make_pair(tileset->GetID(), tile));
    }

    Tile tile = new CTile(
        "pipe",
        PIPE_START_X, PIPE_START_Y,
        PIPE_WIDTH, PIPE_HEIGHT,
        texManager->GetTexture(TEXTURE_PIPE)
    );
    this->tilesets.insert(make_pair(PIPE_TILESET_ID, tile));
    return;
}

void GameMap::Unload()
{
    for (auto x : tilesets)
    {
        delete x.second;
        x.second = nullptr;
    }
    tilesets.clear();

    for (auto texture : textures)
    {
        texture->Release();
        texture = nullptr;
    }
    textures.clear();

    if (mapData != nullptr) delete mapData;
    mapData = nullptr;
    if (spawnerManager != nullptr) spawnerManager->ClearServices();

    gameObjects.clear();
}

void GameMap::LoadEnvironment()
{
    if (mapData == nullptr) return;
    auto scene = Game::GetInstance().GetService<SceneManager>()->GetActiveScene();
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
                // DebugOut(L"Solid: %f, %f, %f, %f\n", position.x, position.y, size.x, size.y);

                SolidBox* solid = Instantiate<SolidBox>();
                solid->SetPosition(position + (size / 2.0f));
                solid->GetColliders()->at(0)->SetBoxSize(size);
                this->gameObjects.push_back(solid);
                this->objectIDs.push_back(objects->at(i)->id);
                auto boxSize = solid->GetColliders()->at(0)->GetBoxSize();
                // DebugOut(L"BoxSize: %f,%f,%f,%f\n", solid->GetTransform().Position.x, solid->GetTransform().Position.y, boxSize.x, boxSize.y);
            }
        }

        if (groupName.compare("Pipes") == 0)
        {
            for (int i = 0; i < objects->size(); ++i)
            {
                Vector2 position(objects->at(i)->x, objects->at(i)->y);
                Vector2 size(objects->at(i)->width, objects->at(i)->height);
                auto type = objects->at(i)->type;

                Pipe* pipe = Instantiate<Pipe>();
                pipe->SetSize(size.x, size.y);
                pipe->SetPosition(position);

                auto props = split(type, "-");
                auto color = props.at(0);
                auto dir = props.at(1);
                int startX = PIPE_START_X, startY = PIPE_START_Y;

                if (color.compare("green") == 0) startX += 2;
                if (dir.compare("up") == 0 || dir.compare("down") == 0 || dir.compare("vertical") == 0)
                {
                    startY += 2;
                    pipe->SetHeadSrc(IntPoint{ startX * 48, startY * 48 },
                        IntPoint{ (startX + 1) * 48, startY * 48 });
                    pipe->SetBodySrc(IntPoint{ startX * 48, (startY + 1) * 48 },
                        IntPoint{ (startX + 1) * 48, (startY + 1) * 48 });

                    if (dir.compare("up") == 0) pipe->SetDirection(PipeDirection::Up);
                    else if (dir.compare("vertical") == 0) pipe->SetDirection(PipeDirection::Vertical);
                    else if (dir.compare("down") == 0) pipe->SetDirection(PipeDirection::Down);
                }
                else
                {
                    pipe->SetHeadSrc(IntPoint{ startX * 48, startY * 48 },
                        IntPoint{ startX * 48, (startY + 1) * 48 });
                    pipe->SetBodySrc(IntPoint{ (startX + 1) * 48, startY * 48 },
                        IntPoint{ (startX + 1) * 48, (startY + 1) * 48 });
                }
                pipe->SetTileset(GetPipeTileset());

                this->gameObjects.push_back(pipe);
                this->objectIDs.push_back(objects->at(i)->id);
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
                this->objectIDs.push_back(objects->at(i)->id);

                auto boxSize = solid->GetColliders()->at(0)->GetBoxSize();
                // DebugOut(L"BoxSize: %f,%f,%f,%f\n", solid->GetTransform().Position.x, solid->GetTransform().Position.y, boxSize.x, boxSize.y);
            }
        }

        if (groupName.compare("Warp") == 0)
        {
            for (int i = 0; i < objects->size(); ++i)
            {
                std::string name = objects->at(i)->name;
                std::string type = objects->at(i)->type;
                Vector2 position(objects->at(i)->x, objects->at(i)->y);
                Vector2 size(objects->at(i)->width, objects->at(i)->height);

                if (name.compare("warp-entrance") == 0)
                {
                    WarpEntrance* warp = Instantiate<WarpEntrance>();
                    warp->SetPosition(position + (size / 2.0f));
                    warp->GetColliders()->at(0)->SetBoxSize(size);

                    WarpDirection wDir =
                        type.compare("down") == 0 ? WarpDirection::Down :
                        (type.compare("up") == 0 ? WarpDirection::Up :
                            (type.compare("left") == 0 ? WarpDirection::Left :
                                (type.compare("right") == 0 ? WarpDirection::Right :
                                    WarpDirection::None)));

                    warp->SetWarpDirection(wDir);
                    this->gameObjects.push_back(warp);
                    this->objectIDs.push_back(objects->at(i)->id);

                    int cellx = objects->at(i)->cellx;
                    int celly = objects->at(i)->celly;
                    if (scene->GetGrid() != nullptr && cellx != -1 && celly != -1)
                    {
                        scene->GetGrid()->Insert(warp, cellx, celly);
                    }
                }
                else if (name.compare("warp-mark") == 0)
                {
                    WarpMark* warp = Instantiate<WarpMark>();
                    warp->SetPosition(position + (size / 2.0f));
                    warp->GetColliders()->at(0)->SetBoxSize(size);

                    int boundId = stoi(objects->at(i)->GetPropertyValue("camera-bound-id"));
                    int dstX = stoi(objects->at(i)->GetPropertyValue("dest-x"));
                    int dstY = stoi(objects->at(i)->GetPropertyValue("dest-y"));
                    std::string outDir = objects->at(i)->GetPropertyValue("out-direction");
                    int lock = stoi(objects->at(i)->GetPropertyValue("lock"));

                    WarpDirection oDir =
                        outDir.compare("down") == 0 ? WarpDirection::Down :
                        (outDir.compare("up") == 0 ? WarpDirection::Up :
                            (outDir.compare("left") == 0 ? WarpDirection::Left :
                                (outDir.compare("right") == 0 ? WarpDirection::Right :
                                    WarpDirection::None)));

                    warp->SetWarpInfo(boundId, Vector2(dstX, dstY), oDir, lock == 1 ? true : false);
                    this->gameObjects.push_back(warp);
                    this->objectIDs.push_back(objects->at(i)->id);

                    int cellx = objects->at(i)->cellx;
                    int celly = objects->at(i)->celly;
                    if (scene->GetGrid() != nullptr && cellx != -1 && celly != -1)
                    {
                        scene->GetGrid()->Insert(warp, cellx, celly);
                    }
                }
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
                this->objectIDs.push_back(objects->at(i)->id);

                int cellx = objects->at(i)->cellx;
                int celly = objects->at(i)->celly;
                if (scene->GetGrid() != nullptr && cellx != -1 && celly != -1)
                {
                    scene->GetGrid()->Insert(solid, cellx, celly);
                }

                if (name.compare("bcoin") == 0)
                    solid->SetItem({ ItemTags::Coin, type });
                else if (name.compare("bmushroom") == 0)
                    solid->SetItem({ ItemTags::Mushroom, type });
                else if (name.compare("bleaf") == 0)
                    solid->SetItem({ ItemTags::Leaf, type });
                else if (name.compare("b1up-mushroom") == 0)
                    solid->SetItem({ ItemTags::Green1UP, type });
            }
        }

        if (groupName.compare("Bricks") == 0)
        {
            for (int i = 0; i < objects->size(); ++i)
            {
                auto name = objects->at(i)->name;

                Vector2 position(objects->at(i)->x, objects->at(i)->y);

                GameObject solid = nullptr;
                if (name.compare("item") == 0)
                {
                    auto itemBrick = Instantiate<ItemBrick>();
                    solid = itemBrick;
                    auto type = objects->at(i)->type;
                    if (type.compare("p-switch") == 0)
                        itemBrick->SetItem({ ItemTags::PSwitch, 1 });
                }
                else if (!name.empty())
                {
                    auto itemBrick = Instantiate<ItemBrick>();
                    solid = itemBrick;
                    auto type = stoi(objects->at(i)->type);

                    if (name.compare("bcoin") == 0)
                        itemBrick->SetItem({ ItemTags::Coin, type });
                    else if (name.compare("bmushroom") == 0)
                        itemBrick->SetItem({ ItemTags::Mushroom, type });
                    else if (name.compare("bleaf") == 0)
                        itemBrick->SetItem({ ItemTags::Leaf, type });
                    else if (name.compare("b1up-mushroom") == 0)
                        itemBrick->SetItem({ ItemTags::Green1UP, type });
                }
                else
                    solid = Instantiate<Brick>();

                solid->SetPosition(position);
                this->gameObjects.push_back(solid);
                this->objectIDs.push_back(objects->at(i)->id);

                int cellx = objects->at(i)->cellx;
                int celly = objects->at(i)->celly;
                if (scene->GetGrid() != nullptr && cellx != -1 && celly != -1)
                {
                    scene->GetGrid()->Insert(solid, cellx, celly);
                }
            }
        }

        if (groupName.compare("Items") == 0)
        {
            for (int i = 0; i < objects->size(); ++i)
            {
                auto name = objects->at(i)->name;
                Vector2 position(objects->at(i)->x, objects->at(i)->y);
                auto type = objects->at(i)->type;

                GameObject object = nullptr;
                if (name.compare("coin") == 0)
                {
                    if (type.compare("basic") == 0)
                    {
                        auto oid = objects->at(i)->id;
                        auto coin = Instantiate<Coin>();
                        coin->SetPosition(position);
                        this->gameObjects.push_back(coin);
                        this->objectIDs.push_back(oid);
                        object = coin;
                    }
                }
                else if (name.compare("reward") == 0)
                {
                    auto reward = Instantiate<LevelReward>();
                    reward->SetPosition(position);
                    this->gameObjects.push_back(reward);
                    this->objectIDs.push_back(objects->at(i)->id);
                    object = reward;
                }

                if (object != nullptr)
                {
                    int cellx = objects->at(i)->cellx;
                    int celly = objects->at(i)->celly;
                    if (scene->GetGrid() != nullptr && cellx != -1 && celly != -1)
                    {
                        scene->GetGrid()->Insert(object, cellx, celly);
                    }
                }
            }
        }

        if (groupName.compare("MovingPlatforms") == 0)
        {
            for (int i = 0; i < objects->size(); ++i)
            {
                auto name = objects->at(i)->name;
                Vector2 position(objects->at(i)->x, objects->at(i)->y);
                auto type = objects->at(i)->type;

                if (name.compare("platform") == 0)
                {
                    if (type.compare("gravity") == 0)
                    {
                        auto oid = objects->at(i)->id;
                        auto obj = Instantiate<GravityPlatform>();
                        auto startVelX = stof(objects->at(i)->GetPropertyValue("start-velocity-x"));
                        auto startVelY = stof(objects->at(i)->GetPropertyValue("start-velocity-y"));
                        obj->SetPosition(position);
                        obj->SetStartVelocity(Vector2(startVelX, startVelY));
                        obj->GetRigidbody()->SetVelocity(&Vector2(startVelX, startVelY));
                        this->gameObjects.push_back(obj);
                        this->objectIDs.push_back(oid);
                    }
                }
            }
        }

#pragma region World Map
        if (groupName.compare("AnimatedBG") == 0)
        {
            for (int i = 0; i < objects->size(); ++i)
            {
                Vector2 position(objects->at(i)->x, objects->at(i)->y);
                auto name = objects->at(i)->name;

                GameObject obj = nullptr;

                if (name.compare("tree") == 0)
                    obj = Instantiate<MapTree>();

                if (obj != nullptr)
                {
                    obj->SetPosition(position);
                    this->gameObjects.push_back(obj);
                    this->objectIDs.push_back(objects->at(i)->id);
                }
            }
        }

        if (groupName.compare("WorldGraph") == 0)
        {
            Graph* mapGraph = new Graph();

            for (int i = 0; i < objects->size(); ++i)
            {
                Vector2 position(objects->at(i)->x, objects->at(i)->y);
                auto name = objects->at(i)->name;
                auto type = split(objects->at(i)->type, "-");

                int nodeId = stoi(objects->at(i)->GetPropertyValue("node_id"));
                auto adj = split(objects->at(i)->GetPropertyValue("adjacent_list"), ",");
                auto weight = split(objects->at(i)->GetPropertyValue("adjacent_weight"), ",");
                auto linkedScene = objects->at(i)->GetPropertyValue("scene");

                GraphNode* node = new GraphNode(nodeId);
                node->SetPosition(position);
                node->SetSceneID(linkedScene);
                auto adjList = node->GetAdjacentList();
                for (int i = 0; i < adj.size(); ++i)
                {
                    auto id = stoi(adj[i]);
                    Edge edge;
                    edge.nodeID = id;
                    Weight w = Weight::None;
                    if (weight[i].compare("l") == 0) w = Weight::Left;
                    else if (weight[i].compare("r") == 0) w = Weight::Right;
                    else if (weight[i].compare("u") == 0) w = Weight::Up;
                    else if (weight[i].compare("d") == 0) w = Weight::Down;
                    edge.weight = w;
                    adjList->push_back(edge);
                }

                mapGraph->InsertNode(node);

                if (type.at(0).compare("num") == 0)
                {
                    int number = stoi(type.at(1));
                    auto gate = Instantiate<Gate>();
                    gate->SetNumber(number);
                    gate->SetPosition(position);
                    this->gameObjects.push_back(gate);
                    this->objectIDs.push_back(objects->at(i)->id);
                }
                else if (type.at(0).compare("bonus") == 0)
                {
                    if (type.at(1).compare("slot") == 0)
                    {
                        auto gate = Instantiate<Gate>();
                        gate->SetNumber(7);
                        gate->SetPosition(position);
                        this->gameObjects.push_back(gate);
                        this->objectIDs.push_back(objects->at(i)->id);
                    }
                }
            }

            Scene* scene = Game::GetInstance().GetService<SceneManager>()->GetBeingLoadedScene();
            if (scene != nullptr)
            {
                WorldMapScene* worldMapScene = dynamic_cast<WorldMapScene*>(scene);
                if (worldMapScene != nullptr)
                {
                    worldMapScene->GetMarioLocator()->SetMap(mapGraph);
                }
            }
        }
#pragma endregion
    }

    for (auto o : gameObjects) scene->AddObject(o);
}

void GameMap::LoadEnemy()
{
    if (mapData == nullptr) return;
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

                int cellx = objects->at(i)->cellx;
                int celly = objects->at(i)->celly;

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
                        goombaSpawner->Spawn("enm-tan-goomba", position, true, cellx, celly);
                    }
                    else if (type.compare("red-para") == 0)
                        goombaSpawner->Spawn("enm-red-para-goomba", position, true, cellx, celly);
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
                        koopa = koopaSpawner->Spawn("enm-red-koopa", position, true, cellx, celly);
                    else if (type.compare("green") == 0)
                        koopa = koopaSpawner->Spawn("enm-green-koopa", position, true, cellx, celly);
                    else if (type.compare("green-para") == 0)
                        koopa = koopaSpawner->Spawn("enm-green-para-koopa", position, true, cellx, celly);
                    else if (type.compare("red-para") == 0)
                    {
                        koopa = koopaSpawner->Spawn("enm-red-para-koopa", position, true, cellx, celly);
                        RedKoopaParatroopa* para = static_cast<RedKoopaParatroopa*>(koopa);
                        int x = stoi(objects->at(i)->GetPropertyValue("amplitude-x"));
                        int y = stoi(objects->at(i)->GetPropertyValue("amplitude-y"));
                        int t = stoi(objects->at(i)->GetPropertyValue("roundtrip"));
                        auto amplitude = Vector2(0.5f * x, 0.5f * y);
                        para->SetStartPosition(position + amplitude);
                        para->SetPosition(position + amplitude * 2);
                        para->SetAmplitude(amplitude);
                        para->SetRoundtripTime(t);
                    }

                    //if (koopa != nullptr)
                    //{
                    //    auto oid = objects->at(i)->id;
                    //    /*koopa->SetPosition(position);
                    //    koopa->SetPool(koopaSpawner->GetPool());
                    //    koopaSpawner->AddPrototype(oid, new SpawnPrototype(position, koopa));
                    //    this->gameObjects.push_back(koopa);*/
                    //}
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
                        plantSpawner->Spawn("enm-green-piranha-plant", position, true, cellx, celly);
                }
                else if (name.compare("venus-fire-trap") == 0)
                {
                    auto plantSpawner = spawnerManager->GetService<PlantSpawner>();
                    if (plantSpawner == nullptr)
                    {
                        plantSpawner = new PlantSpawner();
                        spawnerManager->AddService(plantSpawner);
                    }

                    VenusFireTrap* venus = nullptr;
                    if (type.compare("red") == 0 || type.compare("green") == 0)
                    {
                        venus = static_cast<VenusFireTrap*>(plantSpawner->Spawn("enm-" + type + "-venus-fire-trap", position, true, cellx, celly));
                    }
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

std::vector<int> GameMap::GetIDs()
{
    return objectIDs;
}

Tile GameMap::GetTileset(int id)
{
    return tilesets.at(id);
}

Tile GameMap::GetPipeTileset()
{
    return tilesets.at(PIPE_TILESET_ID);
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
    DebugOut(L"Gamemap Destroyed");
    /*if (mapData != nullptr) delete mapData;
    spawnerManager->ClearServices();
    delete spawnerManager;*/
}
