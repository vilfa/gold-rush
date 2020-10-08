#ifndef MESH_H
#define MESH_H

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

#include "Shader.h"

/*
* Texture types
*/
enum class TEXTYPEenum
{
    DIFFUSE,
    SPECULAR,
    NORMAL,
    HEIGHT
};

class Mesh
{
public:
    /*
    * Data structure representing a vertex
    */
    struct Vertex
    {
        glm::vec3 position;
        glm::vec3 normal;
        glm::vec2 textureCoordinates;
        glm::vec3 tangent;
        glm::vec3 biTangent;
    };

    /*
    * Data structure representing a texture
    */
    struct Texture
    {
        unsigned int id;
        TEXTYPEenum type;
        std::string path; // Store the path to optimize loading
    };

    std::vector<Mesh::Vertex> Vertices;
    std::vector<unsigned int> Indices; // Indices (indexes) of vertices, representing a mesh
    std::vector<Mesh::Texture> Textures;

    Mesh(std::vector<Mesh::Vertex>& vertices, std::vector<unsigned int>& indices, std::vector<Mesh::Texture>& textures);

    static unsigned int LoadMaterialTextureFromFile(std::string& path, const std::string& directory, bool gamma = false,
        bool flipVertical = true, GLenum textureWrapping = GL_REPEAT,
        GLenum mipmapFilteringMin = GL_LINEAR_MIPMAP_LINEAR, GLenum mipmapFilteringMax = GL_LINEAR);
    void Draw(Shader& shader);

private:
    unsigned int VAO, VBO, EBO;

    static const std::string _TEXTURE_DIFFUSE_NAME;
    static const std::string _TEXTURE_SPECULAR_NAME;
    static const std::string _TEXTURE_NORMAL_NAME;
    static const std::string _TEXTURE_HEIGHT_NAME;

    void setupMesh();
};
#endif // !MESH_H