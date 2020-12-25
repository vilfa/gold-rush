#include "GObject.h"

GObject::GObject(Model obj_model) :
    model_(obj_model)
{
    calculateBoundingBox();
}

void GObject::Draw(Shader& shader)
{
    model_.Draw(shader);
}

void GObject::DrawInstanced(Shader& shader, std::vector<glm::mat4>& instance_mod_mats)
{
    model_.DrawInstanced(shader, instance_mod_mats);
}

void GObject::DrawInstanced(Shader& shader, std::shared_ptr<std::vector<glm::mat4>> instance_mod_mats)
{
    model_.DrawInstanced(shader, instance_mod_mats);
}

AABB GObject::GetBoundingBox()
{
    return bounding_box_;
}

float GObject::GetXMaxAABB()
{
    return bounding_box_.center_position.x + bounding_box_.x_half_dim;
}

float GObject::GetXMinAABB()
{
    return bounding_box_.center_position.x - bounding_box_.x_half_dim;
}

float GObject::GetYMaxAABB()
{
    return bounding_box_.center_position.y + bounding_box_.y_half_dim;
}

float GObject::GetYMinAABB()
{
    return bounding_box_.center_position.y - bounding_box_.y_half_dim;
}

float GObject::GetZMaxAABB()
{
    return bounding_box_.center_position.z + bounding_box_.z_half_dim;
}

float GObject::GetZMinAABB()
{
    return bounding_box_.center_position.z - bounding_box_.z_half_dim;
}

glm::vec3 GObject::GetWorldPosition()
{
    return bounding_box_.center_position;
}

void GObject::SetWorldPosition(glm::vec3 new_world_pos)
{
    world_position_ = new_world_pos;
    //calculateBoundingBox();
}

bool GObject::CollidesAABB(GObject oth_obj)
{
    return (
        (oth_obj.GetXMinAABB() >= GetXMinAABB() && oth_obj.GetXMaxAABB() <= GetXMaxAABB()) &&
        (oth_obj.GetYMinAABB() >= GetYMinAABB() && oth_obj.GetYMaxAABB() <= GetYMaxAABB()) &&
        (oth_obj.GetZMinAABB() >= GetZMinAABB() && oth_obj.GetZMaxAABB() <= GetZMaxAABB())
        );
}

bool GObject::ContainsAABB(glm::vec3 oth_pos)
{
    return (
        (oth_pos.x >= GetXMinAABB() && oth_pos.x <= GetXMaxAABB()) &&
        (oth_pos.y >= GetYMinAABB() && oth_pos.y <= GetYMaxAABB()) &&
        (oth_pos.z >= GetZMinAABB() && oth_pos.z <= GetZMaxAABB())
        );
}

void GObject::calculateBoundingBox()
{
    float min_x, min_y, min_z, max_x, max_y, max_z;
    min_x = min_y = min_z = std::numeric_limits<float>::max();
    max_x = max_y = max_z = std::numeric_limits<float>::min();

    glm::vec3 curr_vert;
    for (std::size_t i = 0; i < model_.meshes_.size(); i++)
    {
        Mesh& curr_mesh = model_.meshes_.at(i);
        for (std::size_t j = 0; j < curr_mesh.vertices_.size(); j++)
        {
            curr_vert = curr_mesh.vertices_.at(i).position;

            float x, y, z;
            x = curr_vert.x;
            y = curr_vert.y;
            z = curr_vert.z;

            if (x < min_x) min_x = x; else if (x > max_x) max_x = x;
            if (y < min_y) min_y = y; else if (y > max_y) max_y = y;
            if (z < min_z) min_z = z; else if (z > max_z) max_z = z;
        }
    }

    float half_x, half_y, half_z, center_x, center_y, center_z;

    half_x = (max_x - min_x) / 2;
    half_y = (max_y - min_y) / 2;
    half_z = (max_z - min_z) / 2;
    center_x = min_x + half_x;
    center_y = min_y + half_y;
    center_z = min_z + half_z;
    bounding_box_.center_position = glm::vec3(center_x, center_y, center_z);
    bounding_box_.x_half_dim = half_x;
    bounding_box_.y_half_dim = half_y;
    bounding_box_.z_half_dim = half_z;
}
