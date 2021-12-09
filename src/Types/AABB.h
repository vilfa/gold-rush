#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class AABB
{
public:
    glm::vec3 center_position;
    float x_half_dim;
    float y_half_dim;
    float z_half_dim;

    AABB();
    AABB(glm::vec3 center_pos, float half_dim);
    AABB(glm::vec3 center_pos, float half_x, float half_y, float half_z);

    glm::vec3 GetCenter();
    float XMin();
    float YMax();
    float ZMin();
    float XMax();
    float YMin();
    float ZMax();

    bool Contains(glm::vec3 oth_pos);
    bool Collides(AABB oth_bbx);
};

inline AABB::AABB() : center_position(glm::vec3(0.0f)),
                      x_half_dim(0.0f),
                      y_half_dim(0.0f),
                      z_half_dim(0.0f)
{
}

inline AABB::AABB(glm::vec3 center_pos, float half_dim) : center_position(center_pos),
                                                          x_half_dim(half_dim),
                                                          y_half_dim(half_dim),
                                                          z_half_dim(half_dim)
{
}

inline AABB::AABB(glm::vec3 center_pos, float half_x,
                  float half_y, float half_z) : center_position(center_pos),
                                                x_half_dim(half_x),
                                                y_half_dim(half_y),
                                                z_half_dim(half_z)
{
}

inline bool AABB::Contains(glm::vec3 oth_pos)
{
    return (oth_pos.x >= XMin() && oth_pos.x <= XMax() &&
            oth_pos.z >= ZMin() && oth_pos.z <= ZMax());
    //&& (oth_pos.y >= YMin() && oth_pos.y <= YMax()));
}

inline bool AABB::Collides(AABB oth_bbx)
{
    return ((XMax() >= oth_bbx.XMin() || XMin() <= oth_bbx.XMax()) &&
            (ZMax() >= oth_bbx.ZMin() || ZMin() <= oth_bbx.ZMax()));
    //&& (oth_bbx.YMin() >= YMin() && oth_bbx.YMax() <= YMax()));
}

inline glm::vec3 AABB::GetCenter()
{
    return center_position;
}

inline float AABB::XMin()
{
    return center_position.x - x_half_dim;
}

inline float AABB::XMax()
{
    return center_position.x + x_half_dim;
}

inline float AABB::YMin()
{
    return center_position.y - y_half_dim;
}

inline float AABB::YMax()
{
    return center_position.y + y_half_dim;
}

inline float AABB::ZMin()
{
    return center_position.z - z_half_dim;
}

inline float AABB::ZMax()
{
    return center_position.z + z_half_dim;
}
