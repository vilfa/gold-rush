#pragma once

#include <iostream>
#include <vector>

#include "Game/Entity.h"
#include "Types/AABB.h"

// This is a point region quadtree implementation.
// Also the Y dimension is being ignored, since the game objects
// are static objects positioned beforehand at game initialization.
// Since the player can only move on the terrain level, we only need
// to check X and Z dimensions to determine if a collision has occured.
//
class QuadTree
{
public:
    QuadTree(AABB bounding_box);
    ~QuadTree();

    bool Insert(Entity ent);
    std::vector<Entity> Query(AABB range);

private:
    const uint32_t _node_capacity_;

    AABB bounding_box_;
    std::vector<Entity> entities_;

    QuadTree *quadrant_1_;
    QuadTree *quadrant_2_;
    QuadTree *quadrant_3_;
    QuadTree *quadrant_4_;

    void subdivide();
};
