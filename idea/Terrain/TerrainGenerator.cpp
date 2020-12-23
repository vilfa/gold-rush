#include "Terrain/TerrainGenerator.h"

TerrainGenerator::TerrainGenerator(
    const uint32_t gridSize
) :
    gridSize(gridSize)
{
    generateHeightMap();
    generateGrid();
    generateVertexPositions();
    generateVertexColors();
    generateVegetationPositions();
}

std::vector<glm::vec3>& TerrainGenerator::GetPositions()
{
    return positions;
}

std::vector<glm::vec3>& TerrainGenerator::GetNormals()
{
    return normals;
}

std::vector<glm::vec3>& TerrainGenerator::GetColors()
{
    return colors;
}

std::vector<glm::vec3>& TerrainGenerator::GetTrees()
{
    return treePositions;
}

std::vector<glm::vec3>& TerrainGenerator::GetBushes()
{
    return bushPositions;
}

std::vector<glm::vec3>& TerrainGenerator::GetRocks()
{
    return rockPositions;
}

std::vector<glm::vec3>& TerrainGenerator::GetGrass()
{
    return grassPositions;
}

void TerrainGenerator::generateHeightMap()
{
    heightMap = NoiseGenerator::PerlinNoise2D(gridSize, gridSize, 6);
}

void TerrainGenerator::generateGrid()
{
    for (std::size_t i = 0; i < gridSize; i++)
    {
        for (std::size_t j = 0; j < gridSize; j++)
        {
            float x = (float)i / (float)gridSize;
            float y = (float)j / (float)gridSize;
            float z = heightMap[i * gridSize + j];
            grid.push_back(glm::vec3(x, y, z));
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
    for (std::size_t x = 0; x < gridSize - 1; x++)
    {
        for (std::size_t y = 0; y < gridSize - 1; y++)
        {
            q0 = x * gridSize + y;
            q1 = x * gridSize + (y + 1);
            q2 = (x + 1) * gridSize + y;
            q3 = (x + 1) * gridSize + (y + 1);

            // The indices of each triangle need to be in CCW order, since we've 
            // set GL_CCW as front face, and OpenGL will cull back faces.
            glm::vec3 v0, v1, v2, v3;
            v0 = grid.at(q0);
            v1 = grid.at(q1);
            v2 = grid.at(q2);
            v3 = grid.at(q3);

            glm::vec3 n1, n2;
            n1 = calculateTriangleNormal(v0, v2, v1);
            n2 = calculateTriangleNormal(v2, v3, v1);

            positions.push_back(v0);
            normals.push_back(n1);
            positions.push_back(v2);
            normals.push_back(n1);
            positions.push_back(v1);
            normals.push_back(n1);
            positions.push_back(v2);
            normals.push_back(n2);
            positions.push_back(v3);
            normals.push_back(n2);
            positions.push_back(v1);
            normals.push_back(n2);
        }
    }
}

void TerrainGenerator::generateVertexColors()
{
    glm::vec3 woodlandColor(0.364f, 0.729f, 0.254f);

    for (std::size_t i = 0; i < positions.size(); i+=3)
    {
        colors.push_back(woodlandColor);
        colors.push_back(woodlandColor);
        colors.push_back(woodlandColor);
    }
}

glm::vec3 TerrainGenerator::calculateTriangleNormal(glm::vec3 v0, glm::vec3 v1, glm::vec3 v2)
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
    std::mt19937 reng(std::random_device{}());
    std::vector<glm::vec3> sample;

    std::sample(
        grid.begin(), 
        grid.end(), 
        std::back_inserter(sample),
        gridSize * 35,
        reng
    );
    std::shuffle(
        sample.begin(),
        sample.end(),
        reng
    );

    // Distribute the vegetation positions with the following ratio:
    // 40% trees, 15% bushes, 10% rocks, 35% grass buds.
    std::copy(
        sample.begin(), 
        sample.begin() + sample.size() * 0.4, 
        std::back_inserter(treePositions)
    );
    std::copy(
        sample.begin() + sample.size() * 0.4, 
        sample.begin() + sample.size() * 0.55, 
        std::back_inserter(bushPositions)
    );
    std::copy(
        sample.begin() + sample.size() * 0.55,
        sample.begin() + sample.size() * 0.65,
        std::back_inserter(rockPositions)
    );
    std::copy(
        sample.begin() + sample.size() * 0.65, 
        sample.end(), 
        std::back_inserter(grassPositions)
    );
}