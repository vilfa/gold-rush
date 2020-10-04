#ifndef MODEL_H
#define MODEL_H

#include <string>
#include <vector>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Shader.h"
#include "Mesh.h"
#include "Texture.h"

class Model
{
public:
    Model(const std::string& path, bool gamma = false);

    void Draw(Shader& shader);

private:
    std::vector<Mesh> meshes;
    std::vector<Mesh::Texture> texturesLoaded;
    std::string directory;
    bool gammaCorrection;


    void loadModel(std::string path);
    void processNode(aiNode* node, const aiScene* scene);
    Mesh processMesh(aiMesh* mesh, const aiScene* scene);
    std::vector<Mesh::Texture> loadMaterialTextures(aiMaterial* material,
        aiTextureType aiType, TEXTYPEenum txType);
};
#endif // !MODEL_H