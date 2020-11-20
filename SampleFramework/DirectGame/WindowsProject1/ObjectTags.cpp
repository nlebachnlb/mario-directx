#include "ObjectTags.h"

bool TagUtils::MarioTag(ObjectTags tag)
{
    return tag == ObjectTags::SmallMario || tag == ObjectTags::BigMario || tag == ObjectTags::PowerupMario;
}

bool TagUtils::EnemyTag(ObjectTags tag)
{
    return tag == ObjectTags::Enemy || tag == ObjectTags::Goomba || tag == ObjectTags::KoopasShell;
}

bool TagUtils::ProjectilesTag(ObjectTags tag)
{
    return tag == ObjectTags::FriendlyProjectiles || tag == ObjectTags::HostileProjectiles;
}

bool TagUtils::StaticTag(ObjectTags tag)
{
    return tag == ObjectTags::Solid ||tag == ObjectTags::Block;
}

bool TagUtils::PowerupTag(ObjectTags tag)
{
    return tag == ObjectTags::RedMushroom ||
        tag == ObjectTags::GreenMushroom ||
        tag == ObjectTags::FireFlower ||
        tag == ObjectTags::RaccoonLeaf;

}
