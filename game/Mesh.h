#ifndef MESH_H
#define MESH_H

#include <string>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"

/*
* Texture types
*/
enum class TEXTYPEenum
{
    TEXTURE_DIFFUSE,
    TEXTURE_SPECULAR
};

/*
* Data structure representing a vertex
*/
struct Vertex
{
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 textureCoordinates;
};

/*
* Data structure representing a texture
*/
struct Texture
{
    unsigned int id;
    TEXTYPEenum type;
};

class Mesh
{
public:
    std::vector<Vertex> Vertices;
    std::vector<unsigned int> Indices; // Indices (indexes) of vertices, representing a mesh
    std::vector<Texture> Textures;

    Mesh(std::vector<Vertex>* vertices, std::vector<unsigned int>* indices, std::vector<Texture>* textures);

    void Draw(Shader* shader);

private:
    unsigned int VAO, VBO, EBO;

    static const std::string _TEXTURE_DIFFUSE_NAME;
    static const std::string _TEXTURE_SPECULAR_NAME;

    void setupMesh();
};
#endif