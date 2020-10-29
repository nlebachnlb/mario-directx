#include "ObjectGroup.h"
#include "Utils.h"

ObjectGroup::ObjectGroup()
{
    objects = new std::vector<Object*>();
}

ObjectGroup::~ObjectGroup()
{
    for (auto object : *objects)
        delete object;
    delete objects;
}

ObjectGroup* ObjectGroup::FromXMLData(TiXmlElement* data)
{
    ObjectGroup* objectGroup = new ObjectGroup();
    data->QueryIntAttribute("id", &objectGroup->id);
    objectGroup->name = data->Attribute("name");
    OutputDebugStringW(ToLPCWSTR(objectGroup->name + '\n'));
    
    for (auto node = data->FirstChildElement("object"); node != nullptr; node = node->NextSiblingElement("object"))
    {
        Object* object = new Object();
        node->QueryIntAttribute("id", &object->id);
        node->QueryIntAttribute("x", &object->x);
        node->QueryIntAttribute("y", &object->y);
        if (node->QueryIntAttribute("width",  &object->width) != TIXML_SUCCESS) object->width = 0;
        if (node->QueryIntAttribute("height", &object->height) != TIXML_SUCCESS) object->height = 0;

        auto name = node->Attribute("name");
        if (name != NULL) object->name = name;
        else object->name = "";

        auto type = node->Attribute("type");
        if (type != NULL) object->type = type;
        else object->type = "";

        objectGroup->objects->push_back(object);
    }

    return objectGroup;
}

std::string ObjectGroup::GetName()
{
    return this->name;
}

int ObjectGroup::GetID()
{
    return this->id;
}

std::vector<Object*>* ObjectGroup::GetObjects()
{
    return this->objects;
}
