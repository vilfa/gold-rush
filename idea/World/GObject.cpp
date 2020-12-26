#include "GObject.h"

GObject::GObject(Model obj_model) :
    model_(obj_model)
{
    calculateModelBoundingBox();
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

AABB GObject::GetModelBoundingBox()
{
    return model_bounding_box_;
}

float GObject::GetXMaxModelAABB()
{
    return model_bounding_box_.XMax();
}

float GObject::GetXMinModelAABB()
{
    return model_bounding_box_.XMin();
}

float GObject::GetYMaxModelAABB()
{
    return model_bounding_box_.YMax();
}

float GObject::GetYMinModelAABB()
{
    return model_bounding_box_.YMin();
}

float GObject::GetZMaxModelAABB()
{
    return model_bounding_box_.ZMax();
}

float GObject::GetZMinModelAABB()
{
    return model_bounding_box_.ZMin();
}

void GObject::calculateModelBoundingBox()
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
    model_bounding_box_.center_position = glm::vec3(center_x, center_y, center_z);
    model_bounding_box_.x_half_dim = half_x;
    model_bounding_box_.y_half_dim = half_y;
    model_bounding_box_.z_half_dim = half_z;
}