#include "ObjectTags.h"

bool TagUtils::MarioTag(ObjectTags tag)
{
    return tag == ObjectTags::SmallMario || tag == ObjectTags::BigMario || tag == ObjectTags::PowerupMario;
}

bool TagUtils::ProjectilesTag(ObjectTags tag)
{
    return tag == ObjectTags::FriendlyProjectiles || tag == ObjectTags::HostileProjectiles;
}
