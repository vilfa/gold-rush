#include "Terrain/TerrainGenerator.h"

TerrainGenerator::TerrainGenerator(const uint32_t _grid_size) :
    _grid_size_(_grid_size)
{
    generateHeightMap();
    generateGrid();
    generateVertexPositions();
    generateVertexColors();
    generateVegetationPositions();
}

std::vector<glm::vec3>& TerrainGenerator::GetPositions()
{
    return positions_;
}

std::vector<glm::vec3>& TerrainGenerator::GetNormals()
{
    return normals_;
}

std::vector<glm::vec3>& TerrainGenerator::GetColors()
{
    return colors_;
}

std::vector<glm::vec3>& TerrainGenerator::GetTrees()
{
    return tree_positions_;
}

std::vector<glm::vec3>& TerrainGenerator::GetBushes()
{
    return bush_positions_;
}

std::vector<glm::vec3>& TerrainGenerator::GetRocks()
{
    return rock_positions_;
}

std::vector<glm::vec3>& TerrainGenerator::GetGrass()
{
    return grass_positions_;
}

void TerrainGenerator::generateHeightMap()
{
    height_map_ = NoiseGenerator::PerlinNoise2D(_grid_size_, _grid_size_, 6);
}

void TerrainGenerator::generateGrid()
{
    for (std::size_t i = 0; i < _grid_size_; i++)
    {
        for (std::size_t j = 0; j < _grid_size_; j++)
        {
            float x = (float)i / (float)_grid_size_;
            float y = (float)j / (float)_grid_size_;
            float z = height_map_[i * _grid_size_ + j];
            grid_.push_back(glm::vec3(x, y, z));
        }
    }
}

void TerrainGenerator::generateVertexPositions()
{    
    // Construct quad from unique vertices first, then duplicate vertices.
    // Why do this:
    // a.) there is no need for an index buffer, and
    // b.) each vertex can have its own normal.
    // This does introduce a performance penalty, but I choose to ignore it for now.
    int q0, q1, q2, q3;
    for (std::size_t x = 0; x < _grid_size_ - 1; x++)
    {
        for (std::size_t y = 0; y < _grid_size_ - 1; y++)
        {
            q0 = x * _grid_size_ + y;
            q1 = x * _grid_size_ + (y + 1);
            q2 = (x + 1) * _grid_size_ + y;
            q3 = (x + 1) * _grid_size_ + (y + 1);

            // The indices of each triangle need to be in CCW order, since we've 
            // set GL_CCW as front face, and OpenGL will cull back faces.
            glm::vec3 v0, v1, v2, v3;
            v0 = grid_.at(q0);
            v1 = grid_.at(q1);
            v2 = grid_.at(q2);
            v3 = grid_.at(q3);

            glm::vec3 n1, n2;
            n1 = calculateTriangleNormal(v0, v2, v1);
            n2 = calculateTriangleNormal(v2, v3, v1);

            positions_.push_back(v0);
            normals_.push_back(n1);
            positions_.push_back(v2);
            normals_.push_back(n1);
            positions_.push_back(v1);
            normals_.push_back(n1);
            positions_.push_back(v2);
            normals_.push_back(n2);
            positions_.push_back(v3);
            normals_.push_back(n2);
            positions_.push_back(v1);
            normals_.push_back(n2);
        }
    }
}

void TerrainGenerator::generateVertexColors()
{
    glm::vec3 woodland_color(0.364f, 0.729f, 0.254f);

    for (std::size_t i = 0; i < positions_.size(); i+=3)
    {
        colors_.push_back(woodland_color);
        colors_.push_back(woodland_color);
        colors_.push_back(woodland_color);
    }
}

glm::vec3 TerrainGenerator::calculateTriangleNormal(glm::vec3 v0, glm::vec3 v1, 
    glm::vec3 v2)
{
    glm::vec3 t0 = v0 - v1;
    glm::vec3 t1 = v2 - v1;

    // Calculate triangle normal. We want all three normals to be 
    // the same, so we get the low-poly shading effect
    // when calculating lighting.
    return glm::normalize(glm::cross(t1, t0));
}

void TerrainGenerator::generateVegetationPositions()
{
    std::mt19937 rnd_eng(std::random_device{}());
    std::vector<glm::vec3> sample;

    std::sample(
        grid_.begin(), 
        grid_.end(), 
        std::back_inserter(sample),
        _grid_size_ * 35,
        rnd_eng
    );
    std::shuffle(
        sample.begin(),
        sample.end(),
        rnd_eng
    );

    // Distribute the vegetation positions with the following ratio:
    // 40% trees, 15% bushes, 10% rocks, 35% grass buds.
    std::copy(
        sample.begin(), 
        sample.begin() + sample.size() * 0.4, 
        std::back_inserter(tree_positions_)
    );
    std::copy(
        sample.begin() + sample.size() * 0.4, 
        sample.begin() + sample.size() * 0.55, 
        std::back_inserter(bush_positions_)
    );
    std::copy(
        sample.begin() + sample.size() * 0.55,
        sample.begin() + sample.size() * 0.65,
        std::back_inserter(rock_positions_)
    );
    std::copy(
        sample.begin() + sample.size() * 0.65, 
        sample.end(), 
        std::back_inserter(grass_positions_)
    );
}