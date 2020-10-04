#define STB_IMAGE_IMPLEMENTATION
#include "Mesh.h"

const std::string Mesh::_TEXTURE_DIFFUSE_NAME = "texture_diffuse_";
const std::string Mesh::_TEXTURE_SPECULAR_NAME = "texture_specular_";
const std::string Mesh::_TEXTURE_NORMAL_NAME = "texture_normal_";
const std::string Mesh::_TEXTURE_HEIGHT_NAME = "texture_height_";

Mesh::Mesh(std::vector<Mesh::Vertex>& vertices, std::vector<unsigned int>& indices, std::vector<Mesh::Texture>& textures) :
    Vertices(vertices),
    Indices(indices),
    Textures(textures)
{
    setupMesh();
}

unsigned int Mesh::LoadMaterialTextureFromFile(std::string path, const std::string directory, bool gamma, bool flipVertical,
    GLenum textureWrapping, GLenum mipmapFilteringMin, GLenum mipmapFilteringMax)
{
    std::string fullPath = directory + "/" + path;

    unsigned int textureId;

    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);

    /*
    * Load and generate texture.
    */
    if (flipVertical)
    {
        stbi_set_flip_vertically_on_load(true);
    }

    int width, height, nChannels;
    unsigned char* data = stbi_load(fullPath.c_str(), &width, &height, &nChannels, 0);
    if (data)
    {
        GLenum format = GL_RGB;
        if (nChannels == 1)
        {
            format = GL_RED;
        }
        else if (nChannels == 3)
        {
            format = GL_RGB;
        }
        else if (nChannels == 4)
        {
            format = GL_RGBA;
        }

        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        /*
        * Set the wrapping and filtering options (on currently bound texture object).
        */
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, textureWrapping);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, textureWrapping);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, mipmapFilteringMin);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mipmapFilteringMax);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "ERROR::MESH::LOAD_TEXTURE_FROM_FILE::FILE_READ_ERROR" << std::endl;
        std::cout << "Path:" << path << std::endl;
        stbi_image_free(data);
    }

    return textureId;
}

void Mesh::Draw(Shader& shader)
{
    unsigned int diffuseCount = 1;
    unsigned int specularCount = 1;
    unsigned int normalCount = 1;
    unsigned int heightCount = 1;

    for (std::size_t i = 0; i < Textures.size(); i++)
    {
        glActiveTexture(GL_TEXTURE0 + (GLint)i);

        std::string textureNumber;
        TEXTYPEenum textureType;
        std::string textureName;

        try
        {
            textureType = Textures.at(i).type;
        }
        catch (const std::out_of_range& e)
        {
            std::cout << "ERROR::MESH::DRAW::OUT_OF_RANGE_EXCEPTION" << std::endl;
            std::cout << e.what() << std::endl;
        }

        if (textureType == TEXTYPEenum::DIFFUSE)
        {
            textureName = _TEXTURE_DIFFUSE_NAME;
            textureNumber = std::to_string(diffuseCount++);
        }
        else if (textureType == TEXTYPEenum::SPECULAR)
        {
            textureName = _TEXTURE_SPECULAR_NAME;
            textureNumber = std::to_string(specularCount++);
        }
        else if (textureType == TEXTYPEenum::NORMAL)
        {
            textureName = _TEXTURE_NORMAL_NAME;
            textureNumber = std::to_string(normalCount++);
        }
        else if (textureType == TEXTYPEenum::HEIGHT)
        {
            textureName = _TEXTURE_HEIGHT_NAME;
            textureNumber = std::to_string(heightCount++);
        }

        shader.SetInt((textureName + textureNumber), (int)i);
        glBindTexture(GL_TEXTURE_2D, Textures.at(i).id);
    }

    // Draw the mesh
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, Indices.size(), GL_UNSIGNED_INT, 0);

    // Unbind the VAO
    glBindVertexArray(0);

    glActiveTexture(GL_TEXTURE0);
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
    glBufferData(GL_ARRAY_BUFFER, sizeof(Mesh::Vertex) * Vertices.size(), Vertices.data(), GL_STATIC_DRAW);

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
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Mesh::Vertex), 
        (void*)offsetof(Mesh::Vertex, Mesh::Vertex::position));
    // Normal vectors of a vertex
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Mesh::Vertex), 
        (void*)offsetof(Mesh::Vertex, Mesh::Vertex::normal));
    // Texture coordinates of a vertex
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Mesh::Vertex), 
        (void*)offsetof(Mesh::Vertex, Mesh::Vertex::textureCoordinates));

    /*
    * Unbind the vertex attribute array
    */
    glBindVertexArray(0);
}