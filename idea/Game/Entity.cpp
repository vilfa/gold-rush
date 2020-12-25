#include "Entity.h"

Entity::Entity(TerrainElement& terr_el, glm::mat4& world_transform) :
    terrain_element_(terr_el),
    world_transform_(world_transform)
{
    setupBoundingBox();
}

AABB& Entity::GetBoundingBox()
{
    return bounding_box_;
}

bool Entity::Collides(Entity oth_ent)
{
    return (
        (oth_ent.GetXMinAABB() >= GetXMinAABB() && oth_ent.GetXMaxAABB() <= GetXMaxAABB()) &&
        (oth_ent.GetYMinAABB() >= GetYMinAABB() && oth_ent.GetYMaxAABB() <= GetYMaxAABB()) &&
        (oth_ent.GetZMinAABB() >= GetZMinAABB() && oth_ent.GetZMaxAABB() <= GetZMaxAABB())
        );
}

bool Entity::Contains(glm::vec3 oth_pos)
{
    return (
        (oth_pos.x >= GetXMinAABB() && oth_pos.x <= GetXMaxAABB()) &&
        (oth_pos.y >= GetYMinAABB() && oth_pos.y <= GetYMaxAABB()) &&
        (oth_pos.z >= GetZMinAABB() && oth_pos.z <= GetZMaxAABB())
        );
}

float Entity::GetXMaxAABB()
{
    return bounding_box_.center_position.x + bounding_box_.x_half_dim;
}

float Entity::GetXMinAABB()
{
    return bounding_box_.center_position.x - bounding_box_.x_half_dim;
}

float Entity::GetYMaxAABB()
{
    return bounding_box_.center_position.y + bounding_box_.y_half_dim;
}

float Entity::GetYMinAABB()
{
    return bounding_box_.center_position.y - bounding_box_.y_half_dim;
}

float Entity::GetZMaxAABB()
{
    return bounding_box_.center_position.z + bounding_box_.z_half_dim;
}

float Entity::GetZMinAABB()
{
    return bounding_box_.center_position.z - bounding_box_.z_half_dim;
}

void Entity::setupBoundingBox()
{
    AABB ent_bounding_box = terrain_element_.GetModelBoundingBox();
    
    glm::vec4 ent_aabb_center_v4 = glm::vec4(ent_bounding_box.center_position, 1.0f);
    glm::vec3 ent_aabb_center_v3 = glm::vec3(world_transform_ * ent_aabb_center_v4);
    ent_bounding_box.center_position = ent_aabb_center_v3;

    bounding_box_ = ent_bounding_box;
}
