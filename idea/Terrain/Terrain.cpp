#include "Terrain.h"

Terrain::Terrain(const int gridSize) :
    gridSize(gridSize)
{
    TerrainGenerator tg(gridSize);
    setupVertices(tg.GetPositions(), tg.GetNormals(), tg.GetColors());
    setupTerrain();
}

void Terrain::Draw(Shader& shader)
{
    shader.Use();

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, (GLsizei)Vertices.size());
    glBindVertexArray(0);
}

void Terrain::setupVertices(std::vector<glm::vec3>& positions, std::vector<glm::vec3>& normals, std::vector<glm::vec3>& colors)
{
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::translate(model, glm::vec3((float)(-gridSize), (float)(-gridSize), 0.0f));
    model = glm::scale(model, glm::vec3((float)(gridSize * 2), (float)(gridSize * 2), 15.0f));

    for (std::size_t i = 0; i < positions.size(); i++)
    {
        Terrain::Vertex vertex;
        glm::vec4 position = glm::vec4(positions.at(i), 1.0f);
        vertex.position = glm::vec3(model * position);
        vertex.normal = normals.at(i);
        vertex.color = colors.at(i);
        Vertices.push_back(vertex);
    }
}

void Terrain::setupTerrain()
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Terrain::Vertex) * Vertices.size(), Vertices.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Terrain::Vertex),
        (const void*)offsetof(Terrain::Vertex, Terrain::Vertex::position));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Terrain::Vertex),
        (const void*)offsetof(Terrain::Vertex, Terrain::Vertex::normal));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Terrain::Vertex),
        (const void*)offsetof(Terrain::Vertex, Terrain::Vertex::color));

    glBindVertexArray(0);
}
