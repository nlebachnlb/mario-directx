#include "MapData.h"
#include <cmath>
#include "tinyxml.h"

MapData::MapData()
{
    this->tilesets = new std::map<int, TilesetInfo*>();
    this->layers = new std::map<int, Layer*>();
    this->objectGroups = new std::map<int, ObjectGroup*>();
}

MapData::~MapData()
{
    for (auto it = tilesets->begin(); it != tilesets->end(); it++)
        delete it->second;
    delete tilesets;

    for (auto it = layers->begin(); it != layers->end(); it++)
        delete it->second;
    delete layers;

    for (auto it = objectGroups->begin(); it != objectGroups->end(); it++)
        delete it->second;
    delete objectGroups;
}

MapData* MapData::FromTMX(std::string path)
{
    MapData* mapData = new MapData();

    TiXmlDocument document(path.c_str());
    if (document.LoadFile())
    {
        TiXmlElement* root = document.RootElement();
        root->QueryIntAttribute("width",        &mapData->width);
        root->QueryIntAttribute("height",       &mapData->height);
        root->QueryIntAttribute("tilewidth",    &mapData->tileWidth);
        root->QueryIntAttribute("tileheight",   &mapData->tileHeight);

        // Get Tilesets
        for (auto node = root->FirstChildElement("tileset"); node != nullptr; node = node->NextSiblingElement("tileset"))
        {
            TilesetInfo* tileset = TilesetInfo::FromXMLData(node);
            mapData->tilesets->insert(std::make_pair(tileset->GetID(), tileset));
        }

        // Get Layers
        int order = 0;
        for (auto node = root->FirstChildElement("layer"); node != nullptr; node = node->NextSiblingElement("layer"))
        {
            Layer* layer = Layer::FromXMLData(node, order++);
            if (layer->IsVisible() == false) continue;
            mapData->layers->insert(std::make_pair(layer->GetID(), layer));
        }

        // Get Object groups
        for (auto node = root->FirstChildElement("objectgroup"); node != nullptr; node = node->NextSiblingElement("objectgroup"))
        {
            ObjectGroup* group = ObjectGroup::FromXMLData(node);
            mapData->objectGroups->insert(std::make_pair(group->GetID(), group));
        }

        return mapData;
    }

    throw "An error occurred while loading file";
}

void MapData::IntegrateGridData(std::string path)
{
    OutputDebugStringW(ToLPCWSTR("Integrate: " + path + "\n"));
    TiXmlDocument document(path.c_str());
    if (document.LoadFile())
    {
        TiXmlElement* root = document.RootElement();

        // Grid config
        auto config = root->FirstChildElement("config");
        config->QueryIntAttribute("cellwidth", &gridConfig.cellWidth);
        config->QueryIntAttribute("cellheight", &gridConfig.cellHeight);
        config->QueryIntAttribute("width", &gridConfig.width);
        config->QueryIntAttribute("height", &gridConfig.height);

        auto data = root->FirstChildElement("data");
        // Grid data
        for (auto group = data->FirstChildElement("group"); group != nullptr; group = group->NextSiblingElement("group"))
        {
            int id;
            if (group->QueryIntAttribute("id", &id) == TIXML_SUCCESS)
            {
                DebugOut(L"-----Group: %d-----\n", id);
                if (objectGroups->find(id) != objectGroups->end())
                {
                    auto g = objectGroups->at(id);
                    g->IntegrateGridData(group);
                    /*for (auto o : *g->GetObjects())
                        totalObjects.insert(std::make_pair(o->id, o));*/
                }
            }
        }
    }
}

int MapData::GetTilesetIdFromTileId(int tileId)
{
    int result = -1;
    for (auto x : *tilesets)
        if (result < x.first && x.first <= tileId)
            result = x.first;

    return result;
}

int MapData::GetMapWidthInPixels()
{
    return this->width * this->tileWidth;
}

int MapData::GetMapHeightInPixels()
{
    return this->height * this->tileHeight;
}

int MapData::GetMapWidthInTiles()
{
    return this->width;
}

int MapData::GetMapHeightInTiles()
{
    return this->height;
}

int MapData::GetTileWidth()
{
    return this->tileWidth;
}

int MapData::GetTileHeight()
{
    return this->tileHeight;
}

std::map<int, TilesetInfo*>* MapData::GetTilesets()
{
    return this->tilesets;
}

std::map<int, Layer*>* MapData::GetLayers()
{
    return this->layers;
}

std::map<int, ObjectGroup*>* MapData::GetObjectGroups()
{
    return this->objectGroups;
}

//Object* MapData::GetObjectFromID(int id)
//{
//    if (totalObjects.find(id) == totalObjects.end()) return nullptr;
//    return totalObjects.at(id);
//}
