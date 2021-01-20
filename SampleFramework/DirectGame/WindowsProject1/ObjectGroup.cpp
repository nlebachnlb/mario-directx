#include "ObjectGroup.h"
#include "Utils.h"

ObjectGroup::ObjectGroup()
{
    objects = new std::vector<Object*>();
    objectDictionary = new std::unordered_map<int, Object*>();
}

ObjectGroup::~ObjectGroup()
{
    for (auto object : *objects)
        delete object;
    delete objects;
    delete objectDictionary;
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

        // Get custom properties
        auto props = node->FirstChildElement("properties");
        if (props != nullptr)
        {
            for (auto prop = props->FirstChildElement("property"); prop != nullptr; prop = prop->NextSiblingElement("property"))
            {
                std::string propName = prop->Attribute("name");
                std::string propValue = prop->Attribute("value");

                object->properties.insert(std::make_pair(propName, propValue));
                // OutputDebugStringW(ToLPCWSTR("object: " + object->name + ": prop: " + propName + ": " + propValue + "\n"));
            }
        }

        object->cellx = object->celly = -1;
        objectGroup->objects->push_back(object);
        objectGroup->objectDictionary->insert(std::make_pair(object->id, object));
    }

    return objectGroup;
}

void ObjectGroup::IntegrateGridData(TiXmlElement* data)
{
    for (auto object = data->FirstChildElement("object"); object != nullptr; object = object->NextSiblingElement("object"))
    {
        int id;
        if (object->QueryIntAttribute("id", &id) == TIXML_SUCCESS)
        {
            if (objectDictionary->find(id) != objectDictionary->end())
            {
                auto o = objectDictionary->at(id);
                object->QueryIntAttribute("cellx", &o->cellx);
                object->QueryIntAttribute("celly", &o->celly);
                DebugOut(L"object cell: %d, %d\n", o->cellx, o->celly);
            }
        }
    }
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

std::string Object::GetPropertyValue(std::string propertyName)
{
    if (properties.find(propertyName) != properties.end())
        return properties.at(propertyName);
    return "";
}
