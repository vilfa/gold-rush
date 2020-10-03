#include "Mesh.h"

const std::string Mesh::_TEXTURE_DIFFUSE_NAME = "texture_diffuse_";
const std::string Mesh::_TEXTURE_SPECULAR_NAME = "texture_specular_";

Mesh::Mesh(std::vector<Vertex>* vertices, std::vector<unsigned int>* indices, std::vector<Texture>* textures) :
    Vertices(*vertices),
    Indices(*indices),
    Textures(*textures)
{
    setupMesh();
}

void Mesh::Draw(Shader* shader)
{
    unsigned int diffuseCount = 1;
    unsigned int specularCount = 1;

    for (std::size_t i = 0; i < Textures.size(); i++)
    {
        glActiveTexture(GL_TEXTURE0 + i);

        std::string textureNumber;
        TEXTYPEenum textureType;
        std::string textureName;

        try
        {
            textureType = Textures.at(i).type;
        }
        catch (const std::out_of_range const& e)
        {
            std::cout << "ERROR::MESH::DRAW::OUT_OF_RANGE_EXCEPTION" << std::endl;
            std::cout << e.what() << std::endl;
        }

        if (textureType == TEXTYPEenum::TEXTURE_DIFFUSE)
        {
            textureName = _TEXTURE_DIFFUSE_NAME;
            textureNumber = std::to_string(diffuseCount++);
        }
        else if (textureType == TEXTYPEenum::TEXTURE_SPECULAR)
        {
            textureName = _TEXTURE_SPECULAR_NAME;
            textureNumber == std::to_string(diffuseCount++);
        }

        shader->SetInt(("material." + textureName + textureNumber), (int)i);
        glBindTexture(GL_TEXTURE_2D, Textures.at(i).id);
    }
    glActiveTexture(GL_TEXTURE0);

    // Draw the mesh
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, Indices.size(), GL_UNSIGNED_INT, 0);

    // Unbind the VAO
    glBindVertexArray(0);
}

void Mesh::setupMesh()
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    
    /*
    * VERTICES DATA
    * The data of a single vertex is contained in a single Vertex struct, thus the size
    * of the data for all vertices is the sizeof(Vertex) * Vertices.size().
    * Also: prefer container.data() over &container[0]
    */
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * Vertices.size(), Vertices.data(), GL_STATIC_DRAW);

    /*
    * INDICES DATA
    * The indices of vertices in a mesh.
    */
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * Indices.size(), Indices.data(), GL_STATIC_DRAW);

    /*
    * VERTEX ATTRIBUTE ARRAY
    */
    // Position vectors of a vertex
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Vertex::position));
    // Normal vectors of a vertex
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Vertex::normal));
    // Texture coordinates of a vertex
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Vertex::textureCoordinates));

    /*
    * Unbind the vertex attribute array
    */
    glBindVertexArray(0);
}
