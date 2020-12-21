#include "Terrain/TerrainGenerator.h"

TerrainGenerator::TerrainGenerator(const int gridSize) :
    noiseGenerator(NoiseGenerator()),
    gridSize(gridSize),
    heightMap(nullptr)
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
    heightMap = noiseGenerator.PerlinNoise2D(gridSize, gridSize, 4);
}

void TerrainGenerator::generateGrid()
{
    for (int x = 0; x < gridSize; x++)
    {
        for (int y = 0; y < gridSize; y++)
        {
            // Normalize x and y by dividing with grid size.
            float u = (float)x / (float)gridSize;
            float v = (float)y / (float)gridSize;
            float w = heightMap[x * gridSize + y];

            grid.push_back(glm::vec3(u, v, w));
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
    for (int x = 0; x < gridSize - 1; x++)
    {
        for (int y = 0; y < gridSize - 1; y++)
        {
            q0 = x * gridSize + y;
            q1 = x * gridSize + (y + 1);
            q2 = (x + 1) * gridSize + y;
            q3 = (x + 1) * gridSize + (y + 1);

            // The indices of each triangle need to be in CCW order, since we've set
            // the GL_CCW as front face, and OpenGL will cull back faces.
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
    glm::vec3 woodlandColors[] = {
        glm::vec3(0.71f, 0.73f, 0.49f),
        glm::vec3(0.48f, 0.54f, 0.29f),
        glm::vec3(0.59f, 0.65f, 0.38f),
        glm::vec3(0.36f, 0.45f, 0.22f),
        glm::vec3(0.32f, 0.36f, 0.21f),
        glm::vec3(0.26f, 0.34f, 0.17f),
    };

    std::mt19937 engine(std::random_device{}());
    std::uniform_int_distribution<int> distrib(0, 5);

    int colorIndex;
    for (std::size_t i = 0; i < positions.size(); i+=3)
    {
        colorIndex = distrib(engine);
        colors.push_back(woodlandColors[colorIndex]);
        colors.push_back(woodlandColors[colorIndex]);
        colors.push_back(woodlandColors[colorIndex]);
    }
}

glm::vec3 TerrainGenerator::calculateTriangleNormal(glm::vec3 v0, glm::vec3 v1, glm::vec3 v2)
{
    // Calculate triangle vectors.
    glm::vec3 t1 = v1 - v0;
    glm::vec3 t2 = v2 - v0;
    // Calculate triangle normal. We want all three normals to be 
    // the same, so we get the low-poly shading effect
    // when calculating lighting.
    return glm::cross(t1, t2);
}

void TerrainGenerator::generateVegetationPositions()
{
    std::mt19937 engine(std::random_device{}());
    std::vector<glm::vec3> sample;

    std::sample(
        grid.begin(), 
        grid.end(), 
        std::back_inserter(sample),
        5000,
        engine
    );

    std::shuffle(
        sample.begin(),
        sample.end(),
        engine
    );

    // Distribute the vegetation positions with the following ratio:
    // 50% trees, 25% rocks, 25% grass buds.
    std::copy(
        sample.begin(), 
        sample.begin() + (sample.size() / 2), 
        std::back_inserter(treePositions)
    );
    std::copy(
        sample.begin() + (sample.size() / 2), 
        sample.begin() + (sample.size() / 4) * 3, 
        std::back_inserter(rockPositions)
    );
    std::copy(
        sample.begin() + (sample.size() / 4) * 3, 
        sample.end(), 
        std::back_inserter(grassPositions)
    );
}