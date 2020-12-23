#pragma once

#include <string>
#include <vector>

#include <stb/stb_image.h>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Renderer/Shader.h"
#include "Types/ETexture.h"

class Mesh
{
public:
    struct Vertex
    {
        glm::vec3 position;
        glm::vec3 normal;
        glm::vec2 textureCoordinates;
        glm::vec3 tangent;
        glm::vec3 biTangent;
    };

    struct Texture
    {
        uint32_t id;
        glm::vec4 color;
        std::string path;
        TEXTYPEenum type;
        TEXFORMATenum format;
    };

    std::vector<Mesh::Vertex> Vertices;
    std::vector<uint32_t> Indices;
    std::vector<Mesh::Texture> Textures;

    Mesh(
        std::vector<Mesh::Vertex>& vertices, 
        std::vector<uint32_t>& indices, 
        std::vector<Mesh::Texture>& textures,
        bool embedded = false
    );

    static uint32_t LoadTextureFromFile(
        const std::string path, 
        const std::string directory, 
        bool gamma = false,
        bool flipVertical = true, 
        GLenum textureWrapping = GL_REPEAT,
        GLenum mipmapFilteringMin = GL_LINEAR_MIPMAP_LINEAR, 
        GLenum mipmapFilteringMax = GL_LINEAR
    );
    void Draw(Shader& shader);
    void DrawInstanced(Shader& shader, const std::size_t instanceSize);

private:
    uint32_t VAO, VBO, EBO;
    bool embedded;

    static const std::string _TEXTURE_DIFFUSE_NAME;
    static const std::string _TEXTURE_SPECULAR_NAME;
    static const std::string _TEXTURE_NORMAL_NAME;
    static const std::string _TEXTURE_HEIGHT_NAME;
    static const std::string _COLOR_DIFFUSE_NAME;
    static const std::string _COLOR_SPECULAR_NAME;
    static const std::string _COLOR_AMBIENT_NAME;
    static const std::string _COLOR_EMISSIVE_NAME;

    void setupMesh();
    void setupTextures(Shader& shader);
    void setupTexturesEmbedded(Shader& shader);
};