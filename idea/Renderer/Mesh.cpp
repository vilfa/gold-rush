#define STB_IMAGE_IMPLEMENTATION
#include "Renderer/Mesh.h"

const std::string Mesh::_TEXTURE_DIFFUSE_NAME = "texture_diffuse_";
const std::string Mesh::_TEXTURE_SPECULAR_NAME = "texture_specular_";
const std::string Mesh::_TEXTURE_NORMAL_NAME = "texture_normal_";
const std::string Mesh::_TEXTURE_HEIGHT_NAME = "texture_height_";
const std::string Mesh::_COLOR_DIFFUSE_NAME = "color_diffuse_";
const std::string Mesh::_COLOR_SPECULAR_NAME = "color_specular_";
const std::string Mesh::_COLOR_AMBIENT_NAME = "color_ambient_";
const std::string Mesh::_COLOR_EMISSIVE_NAME = "color_emissive_";

Mesh::Mesh(
    std::vector<Mesh::Vertex>& vertices, 
    std::vector<uint32_t>& indices, 
    std::vector<Mesh::Texture>& textures,
    bool embedded
) :
    Vertices(vertices),
    Indices(indices),
    Textures(textures),
    embedded(embedded)
{
    setupMesh();
}

uint32_t Mesh::LoadTextureFromFile(
    const std::string path, 
    const std::string directory, 
    bool gamma, 
    bool flipVertical,
    GLenum textureWrapping, 
    GLenum mipmapFilteringMin, 
    GLenum mipmapFilteringMax
)
{
    const std::string fullPath = directory + "/" + path;

    uint32_t textureId;
    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);

    stbi_set_flip_vertically_on_load(flipVertical);

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
    shader.Use();

    if (embedded)
    {
        setupTextures(shader);
    }
    else
    {
        setupTexturesEmbedded(shader);
    }

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, (GLsizei)Indices.size(), GL_UNSIGNED_INT, (const void*)0);
    glBindVertexArray(0);

    glActiveTexture(GL_TEXTURE0);
}

void Mesh::DrawInstanced(Shader& shader, const std::size_t instanceSize)
{
    shader.Use();

    if (embedded)
    {
        setupTexturesEmbedded(shader);
    }
    else
    {
        setupTextures(shader);
    }
    
    glBindVertexArray(VAO);

    std::size_t sizeOfVec4 = sizeof(glm::vec4);

    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 4 * sizeOfVec4, (const void*)0);
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 4 * sizeOfVec4, (const void*)(1 * sizeOfVec4));
    glEnableVertexAttribArray(5);
    glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 4 * sizeOfVec4, (const void*)(2 * sizeOfVec4));
    glEnableVertexAttribArray(6);
    glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, 4 * sizeOfVec4, (const void*)(3 * sizeOfVec4));

    glVertexAttribDivisor(3, 1);
    glVertexAttribDivisor(4, 1);
    glVertexAttribDivisor(5, 1);
    glVertexAttribDivisor(6, 1);

    glDrawElementsInstanced(
        GL_TRIANGLES, 
        (GLsizei)Indices.size(), 
        GL_UNSIGNED_INT, 
        (const void*)0, 
        (GLsizei)instanceSize
    );

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
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * Indices.size(), Indices.data(), GL_STATIC_DRAW);

    /*
    * VERTEX ATTRIBUTES
    */
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(
        0,
        3,
        GL_FLOAT, 
        GL_FALSE, 
        sizeof(Mesh::Vertex), 
        (const void*)offsetof(Mesh::Vertex, Mesh::Vertex::position)
    );

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(
        1, 
        3, 
        GL_FLOAT, 
        GL_FALSE, 
        sizeof(Mesh::Vertex), 
        (const void*)offsetof(Mesh::Vertex, Mesh::Vertex::normal)
    );

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(
        2, 
        2, 
        GL_FLOAT, 
        GL_FALSE, 
        sizeof(Mesh::Vertex), 
        (const void*)offsetof(Mesh::Vertex, Mesh::Vertex::textureCoordinates)
    );

    glBindVertexArray(0);
}

void Mesh::setupTextures(Shader& shader)
{
    shader.Use();

    uint32_t diffuseCount = 1;
    uint32_t specularCount = 1;
    uint32_t normalCount = 1;
    uint32_t heightCount = 1;

    for (std::size_t i = 0; i < Textures.size(); i++)
    {
        glActiveTexture(GL_TEXTURE0 + (GLint)i);

        TEXTYPEenum textureType;
        std::string textureNumber;
        std::string textureName;

        try
        {
            textureType = Textures.at(i).type;
        }
        catch (const std::out_of_range& e)
        {
            std::cout << "ERROR::MESH::SETUP_TEXTURES::OUT_OF_RANGE_EXCEPTION" << std::endl;
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

        shader.SetInt((textureName + textureNumber), (int&)i);
        glBindTexture(GL_TEXTURE_2D, Textures.at(i).id);
    }
}

void Mesh::setupTexturesEmbedded(Shader& shader)
{
    shader.Use();

    uint32_t diffuseCount = 1;
    uint32_t ambientCount = 1;
    //uint32_t specularCount = 1;
    //uint32_t emissiveCount = 1;

    for (std::size_t i = 0; i < Textures.size(); i++)
    {
        TEXTYPEenum textureType;
        std::string textureNumber;
        std::string textureName;

        try
        {
            textureType = Textures.at(i).type;
        }
        catch (const std::out_of_range& e)
        {
            std::cout << "ERROR::MESH::SETUP_TEXTURES_EMBEDDED::OUT_OF_RANGE_EXCEPTION" << std::endl;
            std::cout << e.what() << std::endl;
        }

        if (textureType == TEXTYPEenum::DIFFUSE)
        {
            textureName = _COLOR_DIFFUSE_NAME;
            textureNumber = std::to_string(diffuseCount++);
        }
        else if (textureType == TEXTYPEenum::AMBIENT)
        {
            textureName = _COLOR_AMBIENT_NAME;
            textureNumber = std::to_string(ambientCount++);
        }
        /*else if (textureType == TEXTYPEenum::SPECULAR)
        {
            textureName = _COLOR_SPECULAR_NAME;
            textureNumber = std::to_string(specularCount++);
        }*/
        /*else if (textureType == TEXTYPEenum::EMISSIVE)
        {
            textureName = _COLOR_EMISSIVE_NAME;
            textureNumber = std::to_string(emissiveCount++);
        }*/

        shader.SetVec4((textureName + textureNumber), Textures.at(i).color);
    }
}
