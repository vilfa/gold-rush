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

bool QuadTree::insert(Entity ent)
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
    }
}

void QuadTree::subdivide()
{

}

void QuadTree::query(AABB range)
{

}
