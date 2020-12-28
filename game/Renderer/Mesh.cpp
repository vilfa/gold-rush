#define STB_IMAGE_IMPLEMENTATION
#include "Renderer/Mesh.h"

const std::string Mesh::_TEXTURE_DIFFUSE_NAME_ = "texture_diffuse_";
const std::string Mesh::_TEXTURE_SPECULAR_NAME_ = "texture_specular_";
const std::string Mesh::_TEXTURE_NORMAL_NAME_ = "texture_normal_";
const std::string Mesh::_TEXTURE_HEIGHT_NAME_ = "texture_height_";
const std::string Mesh::_COLOR_DIFFUSE_NAME_ = "color_diffuse_";
const std::string Mesh::_COLOR_SPECULAR_NAME_ = "color_specular_";
const std::string Mesh::_COLOR_AMBIENT_NAME_ = "color_ambient_";
const std::string Mesh::_COLOR_EMISSIVE_NAME_ = "color_emissive_";

Mesh::Mesh(std::vector<Mesh::Vertex>& vertices, 
    std::vector<uint32_t>& indices, 
    std::vector<Mesh::Texture>& textures,
    bool embedded) :
    vertices_(vertices),
    indices_(indices),
    textures_(textures),
    embedded_(embedded)
{
    setupMesh();
}

uint32_t Mesh::LoadTextureFromFile(const std::string _path, const std::string _directory,
    bool gamma, bool flip_vertical,
    GLenum texture_wrapping, GLenum mipmap_filtering_min,
    GLenum mipmap_filtering_max)
{
    const std::string _full_path = _directory + "/" + _path;

    uint32_t texture_id;
    glGenTextures(1, &texture_id);
    glBindTexture(GL_TEXTURE_2D, texture_id);

    stbi_set_flip_vertically_on_load(flip_vertical);

    int width, height, n_channels;
    unsigned char* data = stbi_load(_full_path.c_str(), &width, &height, &n_channels, 0);
    if (data)
    {
        GLenum format = GL_RGB;
        if (n_channels == 1)
        {
            format = GL_RED;
        }
        else if (n_channels == 3)
        {
            format = GL_RGB;
        }
        else if (n_channels == 4)
        {
            format = GL_RGBA;
        }

        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        // Set the wrapping and filtering options (on the currently bound texture object).
        //
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, texture_wrapping);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, texture_wrapping);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, mipmap_filtering_min);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mipmap_filtering_max);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "ERROR::MESH::LOAD_TEXTURE_FROM_FILE::FILE_READ_ERROR" << std::endl;
        std::cout << "Path:" << _path << std::endl;
        stbi_image_free(data);
    }

    return texture_id;
}

void Mesh::Draw(Shader& shader)
{
    shader.Use();

    if (embedded_)
    {
        setupTexturesEmbedded(shader);
    }
    else
    {
        setupTextures(shader);
    }

    glBindVertexArray(vao_);
    glDrawElements(GL_TRIANGLES, (GLsizei)indices_.size(), GL_UNSIGNED_INT, (const void*)0);
    glBindVertexArray(0);

    glActiveTexture(GL_TEXTURE0);
}

void Mesh::DrawInstanced(Shader& shader, const std::size_t _instance_size)
{
    shader.Use();

    if (embedded_)
    {
        setupTexturesEmbedded(shader);
    }
    else
    {
        setupTextures(shader);
    }
    
    glBindVertexArray(vao_);

    std::size_t size_of_vec4 = sizeof(glm::vec4);

    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 4 * size_of_vec4, (const void*)0);
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 4 * size_of_vec4, (const void*)(1 * size_of_vec4));
    glEnableVertexAttribArray(5);
    glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 4 * size_of_vec4, (const void*)(2 * size_of_vec4));
    glEnableVertexAttribArray(6);
    glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, 4 * size_of_vec4, (const void*)(3 * size_of_vec4));

    glVertexAttribDivisor(3, 1);
    glVertexAttribDivisor(4, 1);
    glVertexAttribDivisor(5, 1);
    glVertexAttribDivisor(6, 1);

    glDrawElementsInstanced(
        GL_TRIANGLES, 
        (GLsizei)indices_.size(), 
        GL_UNSIGNED_INT, 
        (const void*)0, 
        (GLsizei)_instance_size
    );

    glBindVertexArray(0);

    glActiveTexture(GL_TEXTURE0);
}

void Mesh::setupMesh()
{
    glGenVertexArrays(1, &vao_);
    glGenBuffers(1, &vbo_);
    glGenBuffers(1, &ebo_);

    glBindVertexArray(vao_);
    
    
    // VERTICES DATA
    // The data of a single vertex is contained in a single Vertex struct, thus the size
    // of the data for all vertices is the sizeof(Vertex) * Vertices.size().
    // Also: prefer container.data() over &container[0]
    //
    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Mesh::Vertex) * vertices_.size(), vertices_.data(), GL_STATIC_DRAW);

    // INDICES DATA
    // The indices of vertices in a mesh.
    //
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * indices_.size(), indices_.data(), GL_STATIC_DRAW);

    // VERTEX ATTRIBUTES
    //
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
        (const void*)offsetof(Mesh::Vertex, Mesh::Vertex::texture_coords)
    );

    glBindVertexArray(0);
}

void Mesh::setupTextures(Shader& shader)
{
    shader.Use();

    uint32_t diffuse_count = 1;
    uint32_t specular_count = 1;
    uint32_t normal_count = 1;
    uint32_t height_count = 1;

    for (std::size_t i = 0; i < textures_.size(); i++)
    {
        glActiveTexture(GL_TEXTURE0 + (GLint)i);

        TEXTYPEenum texture_type;
        std::string texture_number;
        std::string texture_name;

        try
        {
            texture_type = textures_.at(i).type;
        }
        catch (const std::out_of_range& e)
        {
            std::cout << "ERROR::MESH::SETUP_TEXTURES::OUT_OF_RANGE_EXCEPTION" << std::endl;
            std::cout << e.what() << std::endl;
        }

        if (texture_type == TEXTYPEenum::DIFFUSE)
        {
            texture_name = _TEXTURE_DIFFUSE_NAME_;
            texture_number = std::to_string(diffuse_count++);
        }
        else if (texture_type == TEXTYPEenum::SPECULAR)
        {
            texture_name = _TEXTURE_SPECULAR_NAME_;
            texture_number = std::to_string(specular_count++);
        }
        else if (texture_type == TEXTYPEenum::NORMAL)
        {
            texture_name = _TEXTURE_NORMAL_NAME_;
            texture_number = std::to_string(normal_count++);
        }
        else if (texture_type == TEXTYPEenum::HEIGHT)
        {
            texture_name = _TEXTURE_HEIGHT_NAME_;
            texture_number = std::to_string(height_count++);
        }

        shader.SetInt((texture_name + texture_number), (int&)i);
        glBindTexture(GL_TEXTURE_2D, textures_.at(i).id);
    }
}

void Mesh::setupTexturesEmbedded(Shader& shader)
{
    shader.Use();

    uint32_t diffuse_count = 1;
    uint32_t ambient_count = 1;
    //uint32_t specular_count = 1;
    //uint32_t emissive_count = 1;

    for (std::size_t i = 0; i < textures_.size(); i++)
    {
        TEXTYPEenum texture_type;
        std::string texture_number;
        std::string texture_name;

        try
        {
            texture_type = textures_.at(i).type;
        }
        catch (const std::out_of_range& e)
        {
            std::cout << "ERROR::MESH::SETUP_TEXTURES_EMBEDDED::OUT_OF_RANGE_EXCEPTION" << std::endl;
            std::cout << e.what() << std::endl;
        }

        if (texture_type == TEXTYPEenum::DIFFUSE)
        {
            texture_name = _COLOR_DIFFUSE_NAME_;
            texture_number = std::to_string(diffuse_count++);
        }
        else if (texture_type == TEXTYPEenum::AMBIENT)
        {
            texture_name = _COLOR_AMBIENT_NAME_;
            texture_number = std::to_string(ambient_count++);
        }
        //else if (texture_type == TEXTYPEenum::SPECULAR)
        //{
        //    texture_name = _COLOR_SPECULAR_NAME_;
        //    texture_number = std::to_string(specular_count++);
        //}
        //else if (texture_type == TEXTYPEenum::EMISSIVE)
        //{
        //    texture_name = _COLOR_EMISSIVE_NAME_;
        //    texture_number = std::to_string(emissive_count++);
        //}

        shader.SetVec4((texture_name + texture_number), textures_.at(i).color);
    }
}
