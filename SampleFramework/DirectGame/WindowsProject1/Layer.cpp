#include "Layer.h"
#include "Utils.h"

Layer::Layer()
{
    data = new std::vector<vector<int>*>();
}

Layer::~Layer()
{
    for (auto column : *data)
        delete column;
    delete data;
}

Layer* Layer::FromXMLData(TiXmlElement* data)
{
    Layer* layer = new Layer();

    data->QueryIntAttribute("id", &layer->id);
    layer->name  = data->Attribute("name");
    data->QueryIntAttribute("width",    &layer->width);
    data->QueryIntAttribute("height",   &layer->height);

    std::string dataContent = data->FirstChildElement("data")->GetText();
    std::vector<std::string> tilemap = split(dataContent, ",");

    for (int x = 0; x < layer->width; ++x)
    {
        std::vector<int>* column = new std::vector<int>();
        for (int y = 0; y < layer->height; ++y)
            column->push_back(stoi(tilemap.at(x + y * layer->width)));
        
        layer->data->push_back(column);
    }

    return layer;
}

int Layer::GetID()
{
    return this->id;
}

std::string Layer::GetName()
{
    return this->name;
}

int Layer::GetWidth()
{
    return this->width;
}

int Layer::GetHeight()
{
    return this->height;
}

int Layer::GetTileID(int x, int y)
{
    return this->data->at(x)->at(y);
}
