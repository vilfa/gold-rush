#include "Entity.h"

Entity::Entity(TerrainElement& terr_el, glm::mat4& world_transform) :
    terrain_element_(terr_el),
    world_transform_(world_transform)
{
    setupBoundingBox();
}

AABB Entity::GetBoundingBox()
{
    return bounding_box_;
}

bool Entity::Collides(Entity oth_ent)
{
    return bounding_box_.Collides(oth_ent.bounding_box_);
}

bool Entity::Contains(glm::vec3 oth_pos)
{
    return bounding_box_.Contains(oth_pos);
}

glm::vec3 Entity::GetCenter()
{
    return bounding_box_.GetCenter();
}

float Entity::GetXMaxAABB()
{
    return bounding_box_.XMax();
}

float Entity::GetXMinAABB()
{
    return bounding_box_.XMin();
}

float Entity::GetYMaxAABB()
{
    return bounding_box_.YMax();
}

float Entity::GetYMinAABB()
{
    return bounding_box_.YMin();
}

float Entity::GetZMaxAABB()
{
    return bounding_box_.ZMax();
}

float Entity::GetZMinAABB()
{
    return bounding_box_.ZMin();
}

void Entity::setupBoundingBox()
{
    AABB ent_bounding_box = terrain_element_.GetModelBoundingBox();
    
    glm::vec4 ent_aabb_center_v4 = glm::vec4(ent_bounding_box.center_position, 1.0f);
    glm::vec3 ent_aabb_center_v3 = glm::vec3(world_transform_ * ent_aabb_center_v4);
    ent_bounding_box.center_position = ent_aabb_center_v3;

    bounding_box_ = ent_bounding_box;
}
