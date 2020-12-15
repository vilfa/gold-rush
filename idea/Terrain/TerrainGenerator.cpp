#include "Terrain/TerrainGenerator.h"

TerrainGenerator::TerrainGenerator(const int gridSize) :
    noiseGenerator(NoiseGenerator()),
    gridSize(gridSize),
    heightMap(nullptr)
{
    generateHeightMap();
    generateVertexPositions();
    generateVertexColors();
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

void TerrainGenerator::generateHeightMap()
{
    heightMap = noiseGenerator.PerlinNoise2D(gridSize, gridSize, 4);
}

std::vector<glm::vec3> TerrainGenerator::generateGrid()
{
    std::vector<glm::vec3> positionsUnique;
    for (int x = 0; x < gridSize; x++)
    {
        for (int y = 0; y < gridSize; y++)
        {
            // Normalize x and y by dividing with grid size.
            float u = (float)x / (float)gridSize;
            float v = (float)y / (float)gridSize;
            float w = heightMap[x * gridSize + y];

            positionsUnique.push_back(glm::vec3(u, v, w));
        }
    }
    return positionsUnique;
}

void TerrainGenerator::generateVertexPositions()
{
    // Construct quad from unique vertices first, then duplicate vertices.
    // Why do this:
    // a.) there is no need for an index buffer, and
    // b.) each vertex can have its own normal.
    // This does introduce a performance penalty, but I choose to ignore it for now.

    int q0, q1, q2, q3;
    std::vector<glm::vec3> grid = generateGrid();

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
            n1 = calculateTriangleNormals(v0, v2, v1);
            n2 = calculateTriangleNormals(v2, v3, v1);

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
    for (std::size_t i = 0; i < positions.size(); i++)
    {
        colors.push_back(glm::vec3(1.0f, 1.0f, 0.0f));
    }
}

glm::vec3 TerrainGenerator::calculateTriangleNormals(glm::vec3 v0, glm::vec3 v1, glm::vec3 v2)
{
    // Calculate triangle vectors.
    glm::vec3 t1 = v1 - v0;
    glm::vec3 t2 = v2 - v1;
    // Calculate triangle normal. We wan't all three normals to be 
    // the same, so we get the low-poly shading effect.
    return glm::cross(t1, t2);
}
