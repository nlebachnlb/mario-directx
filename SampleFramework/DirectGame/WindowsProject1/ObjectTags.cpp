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
