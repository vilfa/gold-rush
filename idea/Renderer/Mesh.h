#pragma once

#include <iostream>
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
        glm::vec2 texture_coords;
        glm::vec3 tangent;
        glm::vec3 bi_tangent;
    };

    struct Texture
    {
        uint32_t id;
        glm::vec4 color;
        std::string path;
        TEXTYPEenum type;
        TEXFORMATenum format;
    };

    std::vector<Mesh::Vertex> vertices_;
    std::vector<uint32_t> indices_;
    std::vector<Mesh::Texture> textures_;

    Mesh(std::vector<Mesh::Vertex>& vertices, 
        std::vector<uint32_t>& indices, 
        std::vector<Mesh::Texture>& textures, 
        bool embedded = false);

    static uint32_t LoadTextureFromFile(const std::string _path, const std::string _directory, 
        bool gamma = false, bool flip_vertical = true, 
        GLenum texture_wrapping = GL_REPEAT, GLenum mipmap_filtering_min = GL_LINEAR_MIPMAP_LINEAR, 
        GLenum mipmap_filtering_max = GL_LINEAR);
    void Draw(Shader& shader);
    void DrawInstanced(Shader& shader, const std::size_t _instance_size);

private:
    uint32_t vao_, vbo_, ebo_;
    bool embedded_;

    static const std::string _TEXTURE_DIFFUSE_NAME_;
    static const std::string _TEXTURE_SPECULAR_NAME_;
    static const std::string _TEXTURE_NORMAL_NAME_;
    static const std::string _TEXTURE_HEIGHT_NAME_;
    static const std::string _COLOR_DIFFUSE_NAME_;
    static const std::string _COLOR_SPECULAR_NAME_;
    static const std::string _COLOR_AMBIENT_NAME_;
    static const std::string _COLOR_EMISSIVE_NAME_;

    void setupMesh();
    void setupTextures(Shader& shader);
    void setupTexturesEmbedded(Shader& shader);
};