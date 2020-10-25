#include "ObjectGroup.h"
#include "Utils.h"

ObjectGroup::ObjectGroup()
{
    objects = new std::vector<Object*>();
}

ObjectGroup::~ObjectGroup()
{
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
        node->QueryIntAttribute("width",  &object->width);
        node->QueryIntAttribute("height", &object->height);
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
