#include "QuadTree.h"

QuadTree::QuadTree(AABB bounding_box) :
    _node_capacity_(4),
    bounding_box_(bounding_box),
    quadrant_1_(nullptr),
    quadrant_2_(nullptr),
    quadrant_3_(nullptr),
    quadrant_4_(nullptr)
{
}

QuadTree::~QuadTree()
{
    delete quadrant_1_;
    delete quadrant_2_;
    delete quadrant_3_;
    delete quadrant_4_;
}

bool QuadTree::Insert(Entity ent)
{
    if (!bounding_box_.Contains(ent.GetCenter()))
    {
        return false;
    }

    if (entities_.size() < _node_capacity_)
    {
        entities_.push_back(ent);
        return true;
    }

    if (entities_.size() == _node_capacity_)
    {
        subdivide();

        for (std::size_t i = 0; i < entities_.size(); i++)
        {
            if (quadrant_1_->Insert(entities_.at(i))) continue;
            if (quadrant_2_->Insert(entities_.at(i))) continue;
            if (quadrant_3_->Insert(entities_.at(i))) continue;
            if (quadrant_4_->Insert(entities_.at(i))) continue;
        }
        entities_.clear();

        if (quadrant_1_->Insert(ent)) return true;
        if (quadrant_2_->Insert(ent)) return true;
        if (quadrant_3_->Insert(ent)) return true;
        if (quadrant_4_->Insert(ent)) return true;
    }

    if (quadrant_1_ != nullptr)
    {
        if (quadrant_1_->Insert(ent)) return true;
        if (quadrant_2_->Insert(ent)) return true;
        if (quadrant_3_->Insert(ent)) return true;
        if (quadrant_4_->Insert(ent)) return true;
    }

    return false; // Should never happen.
}

std::vector<Entity> QuadTree::Query(AABB range)
{
    std::vector<Entity> matching_ents;

    if (!bounding_box_.Collides(range))
    {
        return matching_ents;
    }

    for (std::size_t i = 0; i < entities_.size(); i++)
    {
        if (range.Contains(entities_.at(i).bounding_box_.GetCenter()))
        {
            matching_ents.push_back(entities_.at(i));
        }
    }

    if (quadrant_1_ == nullptr)
    {
        return matching_ents;
    }

    std::vector<Entity> match_q1, match_q2, match_q3, match_q4;
    match_q1 = quadrant_1_->Query(range);
    match_q2 = quadrant_2_->Query(range);
    match_q3 = quadrant_3_->Query(range);
    match_q4 = quadrant_4_->Query(range);
    matching_ents.insert(matching_ents.end(), match_q1.begin(), match_q1.end());
    matching_ents.insert(matching_ents.end(), match_q2.begin(), match_q2.end());
    matching_ents.insert(matching_ents.end(), match_q3.begin(), match_q3.end());
    matching_ents.insert(matching_ents.end(), match_q4.begin(), match_q4.end());

    return matching_ents;
}

void QuadTree::subdivide()
{
    float _x = bounding_box_.center_position.x;
    float _z = bounding_box_.center_position.z;
    float _h_dim = bounding_box_.x_half_dim / 2;

    quadrant_1_ = new QuadTree(AABB(glm::vec3(_x + _h_dim, 0.0f, _z + _h_dim), _h_dim));
    quadrant_2_ = new QuadTree(AABB(glm::vec3(_x - _h_dim, 0.0f, _z + _h_dim), _h_dim));
    quadrant_3_ = new QuadTree(AABB(glm::vec3(_x - _h_dim, 0.0f, _z - _h_dim), _h_dim));
    quadrant_4_ = new QuadTree(AABB(glm::vec3(_x + _h_dim, 0.0f, _z - _h_dim), _h_dim));
}