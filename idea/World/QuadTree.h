#pragma once

#include <iostream>
#include <vector>

#include "Types/AABB.h"
#include "Game/Entity.h"

// This is a region quadtree implementation.
// Also the Y dimension is being ignored, since the game objects
// are static objects positioned beforehand at game initialization.
// Since the player can only move on the terrain level, we only need
// to check X and Z dimensions to determine if a collision has occured.
//
class QuadTree
{
public:
    QuadTree(AABB bounding_box);

private:
    const uint32_t _node_capacity_;

    AABB bounding_box_;
    std::vector<Entity> entities_;

    QuadTree* quadrant_1_;
    QuadTree* quadrant_2_;
    QuadTree* quadrant_3_;
    QuadTree* quadrant_4_;

    bool insert(Entity ent);
    void subdivide();
    void query(AABB range);
};

