#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stb/stb_image.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/DefaultLogger.hpp>

#include "Renderer/Shader.h"
#include "Renderer/Mesh.h"

class Model
{
public:
    Model(const std::string& path, bool embedded = false, bool gamma = false);

    void Draw(Shader& shader);
    void DrawInstanced(Shader& shader, std::vector<glm::mat4>& instancedModelMatrices);

private:
    std::vector<Mesh> meshes;
    std::vector<Mesh::Texture> texturesLoaded;
    std::string directory;
    bool gammaCorrection;
    bool materialsEmbedded;

    void loadModel(const std::string& path);
    void processNode(aiNode* node, const aiScene* scene);
    Mesh processMesh(aiMesh* mesh, const aiScene* scene);
    std::vector<Mesh::Texture> loadMaterialTextures(aiMaterial* material,
        aiTextureType aiType, 
        TEXTYPEenum txType, 
        TEXFORMATenum txFormat);
    std::vector<Mesh::Texture> loadMaterialTexturesEmbedded(aiMaterial* material,
        aiTextureType aiType,
        TEXFORMATenum txFormat);
};