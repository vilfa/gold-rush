#include "Renderer/Model.h"

Model::Model(const std::string _path, bool embedded, bool gamma)
    : textures_embedded_(embedded), gamma_correction_(gamma)
{
    double time = glfwGetTime();
    std::cout << "INFO::MODEL::MODEL::BEGIN_LOAD" << std::endl;
    std::cout << "INFO::MODEL::LOAD_MODEL::ASSIMP_LOG" << std::endl;

    // Create logger and attach it to the default output stream.
    //
    Assimp::DefaultLogger::create(NULL, Assimp::Logger::VERBOSE, aiDefaultLogStream_STDOUT);

    loadModel(_path);

    std::cout << "INFO::MODEL::MODEL::END_LOAD" << std::endl;
    std::cout << "Load took:" << (glfwGetTime() - time) * 1000 << "ms" << std::endl;
}

void Model::Draw(Shader &shader)
{
    for (std::size_t i = 0; i < meshes_.size(); i++)
    {
        meshes_[i].Draw(shader);
    }
}

void Model::DrawInstanced(Shader &shader, std::vector<glm::mat4> &instance_mod_mats)
{
    std::size_t instance_size = instance_mod_mats.size();

    uint32_t mat_vbo_id;
    glGenBuffers(1, &mat_vbo_id);
    glBindBuffer(GL_ARRAY_BUFFER, mat_vbo_id);
    glBufferData(GL_ARRAY_BUFFER,
                 instance_size * sizeof(glm::mat4),
                 &instance_mod_mats[0],
                 GL_STATIC_DRAW);

    for (std::size_t i = 0; i < meshes_.size(); i++)
    {
        meshes_[i].DrawInstanced(shader, instance_size);
    }

    // This is important to avoid infinite memory allocation!
    //
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glDeleteBuffers(1, &mat_vbo_id);
}

void Model::DrawInstanced(Shader &shader, std::shared_ptr<std::vector<glm::mat4>> instance_mod_mats)
{
    std::size_t instance_size = instance_mod_mats->size();

    uint32_t mat_vbo_id;
    glGenBuffers(1, &mat_vbo_id);
    glBindBuffer(GL_ARRAY_BUFFER, mat_vbo_id);
    glBufferData(GL_ARRAY_BUFFER,
                 instance_size * sizeof(glm::mat4),
                 instance_mod_mats->data(),
                 GL_STATIC_DRAW);

    for (std::size_t i = 0; i < meshes_.size(); i++)
    {
        meshes_[i].DrawInstanced(shader, instance_size);
    }

    // This is important to avoid infinite memory allocation!
    //
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glDeleteBuffers(1, &mat_vbo_id);
}

void Model::loadModel(const std::string _path)
{
    // Create the importer.
    //
    Assimp::Importer importer;

    // Read the file.
    // Assimp's interface defines each mesh as having an array of faces, where each face
    // is represented by a single primitive. The aiProcess_Triangulate flag defines the
    // primitives as triangles.
    //
    const aiScene *scene = importer.ReadFile(_path, aiProcess_Triangulate | aiProcess_FlipUVs);

    if (!scene || !scene->mRootNode || (scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE))
    {
        std::cout << "ERROR::MODEL::LOAD_MODEL::ASSIMP" << std::endl;
        std::cout << importer.GetErrorString() << std::endl;
        return;
    }

    directory_ = _path.substr(0, _path.find_last_of('/') + 1);
    processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode *node, const aiScene *_scene)
{
    // First process all the node's meshes_ (if any)
    //
    for (std::size_t i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh *mesh = _scene->mMeshes[node->mMeshes[i]];
        meshes_.push_back(processMesh(mesh, _scene));
    }

    // Then process all of the node's children
    //
    for (size_t i = 0; i < node->mNumChildren; i++)
    {
        processNode(node->mChildren[i], _scene);
    }
}

Mesh Model::processMesh(aiMesh *mesh, const aiScene *_scene)
{
    std::vector<Mesh::Vertex> vertices;
    std::vector<uint32_t> indices;
    std::vector<Mesh::Texture> textures;

    // Process the vertices.
    // Construct the position, normal and texture vectors.
    //
    for (std::size_t i = 0; i < mesh->mNumVertices; i++)
    {
        Mesh::Vertex vertex;

        // Construct the position vector for this vertex
        //
        glm::vec3 position_vec;
        position_vec.x = mesh->mVertices[i].x;
        position_vec.y = mesh->mVertices[i].y;
        position_vec.z = mesh->mVertices[i].z;
        vertex.position = position_vec;

        // Construct the normal vector for this vertex
        //
        glm::vec3 normal_vec;
        normal_vec.x = mesh->mNormals[i].x;
        normal_vec.y = mesh->mNormals[i].y;
        normal_vec.z = mesh->mNormals[i].z;
        vertex.normal = normal_vec;

        // If the texture coordinates exist, construct the texture vector for this vertex
        // The index 0 is because Assimp allows for up to 8 differrent sets of texture coordinates
        //
        if (mesh->mTextureCoords[0])
        {
            // Texture coordinates
            //
            glm::vec2 texture_coord_vec;
            texture_coord_vec.x = mesh->mTextureCoords[0][i].x;
            texture_coord_vec.y = mesh->mTextureCoords[0][i].y;
            vertex.texture_coords = texture_coord_vec;

            // Tangent vector to the vertex
            //
            if (mesh->mTangents)
            {
                glm::vec3 tangent_vec;
                tangent_vec.x = mesh->mTangents[i].x;
                tangent_vec.y = mesh->mTangents[i].y;
                tangent_vec.z = mesh->mTangents[i].z;
                vertex.tangent = tangent_vec;
            }

            // Bitangent vector to the vertex (orthogonal to tangent and normal).
            //
            if (mesh->mBitangents)
            {
                glm::vec3 bi_tangent_vec;
                bi_tangent_vec.x = mesh->mBitangents[i].x;
                bi_tangent_vec.y = mesh->mBitangents[i].y;
                bi_tangent_vec.z = mesh->mBitangents[i].z;
                vertex.bi_tangent = bi_tangent_vec;
            }
        }
        else
        {
            vertex.texture_coords = glm::vec2(0.0f, 0.0f);
        }

        vertices.push_back(vertex);
    }

    // Process the mesh faces i.e. the primitives.
    // In this case the primitives are triangles (due to the aiProcess_Triangulate flag).
    // For further explanation look at the Model::loadModel method.
    //
    for (std::size_t i = 0; i < mesh->mNumFaces; i++)
    {
        // Get a face from the mesh
        //
        aiFace face = mesh->mFaces[i];

        // Loop through the indices of this face
        //
        for (std::size_t j = 0; j < face.mNumIndices; j++)
        {
            indices.push_back(face.mIndices[j]);
        }
    }

    if (mesh->mMaterialIndex >= 0)
    {
        aiMaterial *material = _scene->mMaterials[mesh->mMaterialIndex];

        if (textures_embedded_)
        {
            std::vector<Mesh::Texture> embedded_colors =
                loadMaterialTexturesEmbedded(material,
                                             aiTextureType_BASE_COLOR,
                                             TEXFORMATenum::EMBEDDED);
            textures.insert(textures.end(), embedded_colors.begin(), embedded_colors.end());
        }
        else
        {
            // 1. Load the diffuse maps
            //
            std::vector<Mesh::Texture> diffuse_maps = loadMaterialTextures(material,
                                                                           aiTextureType_DIFFUSE,
                                                                           TEXTYPEenum::DIFFUSE,
                                                                           TEXFORMATenum::FILE);
            textures.insert(textures.end(), diffuse_maps.begin(), diffuse_maps.end());

            // 2. Load the specular maps
            //
            std::vector<Mesh::Texture> specular_maps = loadMaterialTextures(material,
                                                                            aiTextureType_SPECULAR,
                                                                            TEXTYPEenum::SPECULAR,
                                                                            TEXFORMATenum::FILE);
            textures.insert(textures.end(), specular_maps.begin(), specular_maps.end());

            // 3. Load the normal maps
            //
            std::vector<Mesh::Texture> normal_maps = loadMaterialTextures(material,
                                                                          aiTextureType_HEIGHT,
                                                                          TEXTYPEenum::NORMAL,
                                                                          TEXFORMATenum::FILE);
            textures.insert(textures.end(), normal_maps.begin(), normal_maps.end());

            // 4. Load the height maps
            //
            std::vector<Mesh::Texture> height_maps = loadMaterialTextures(material,
                                                                          aiTextureType_AMBIENT,
                                                                          TEXTYPEenum::HEIGHT,
                                                                          TEXFORMATenum::FILE);
            textures.insert(textures.end(), height_maps.begin(), height_maps.end());
        }
    }

    return Mesh(vertices, indices, textures, textures_embedded_);
}

std::vector<Mesh::Texture> Model::loadMaterialTextures(aiMaterial *material,
                                                       aiTextureType ai_type,
                                                       TEXTYPEenum tx_type,
                                                       TEXFORMATenum tx_format)
{
    std::cout << "INFO::MODEL::LOAD_MATERIAL_TEXTURES::LOAD_START" << std::endl;
    std::cout << "INFO::MODEL::LOAD_MATERIAL_TEXTURES::TEXTURE_TYPE" << std::endl;
    std::cout << "Texture type:" << (int)tx_type << std::endl;

    std::vector<Mesh::Texture> textures;
    for (std::size_t i = 0; i < material->GetTextureCount(ai_type); i++)
    {
        aiString path;
        material->GetTexture(ai_type, (uint32_t)i, &path);

        bool already_loaded = false;
        for (std::size_t j = 0; j < textures_loaded_.size(); j++)
        {
            if (std::strcmp(textures_loaded_.at(j).path.c_str(), path.C_Str()) == 0)
            {
                textures.push_back(textures_loaded_.at(j));
                already_loaded = true;
                break;
            }
        }

        if (!already_loaded)
        {
            Mesh::Texture texture;
            texture.id =
                Mesh::LoadTextureFromFile(std::string(path.C_Str()), directory_, gamma_correction_);
            texture.type = tx_type;
            texture.path = path.C_Str();
            textures.push_back(texture);
            textures_loaded_.push_back(texture);
        }
    }

    return textures;
}

std::vector<Mesh::Texture> Model::loadMaterialTexturesEmbedded(aiMaterial *material,
                                                               aiTextureType ai_type,
                                                               TEXFORMATenum tx_format)
{
    std::cout << "INFO::MODEL::LOAD_MATERIAL_TEXTURES_EMBEDDED::LOAD_START" << std::endl;

    std::vector<Mesh::Texture> textures;
    aiColor4D diffuse_col, specular_col, ambient_col, emissive_col;
    if (AI_SUCCESS == aiGetMaterialColor(material, AI_MATKEY_COLOR_DIFFUSE, &diffuse_col))
    {
        Mesh::Texture texture;
        texture.color = glm::vec4(diffuse_col.r, diffuse_col.g, diffuse_col.b, diffuse_col.a);
        texture.type = TEXTYPEenum::DIFFUSE;
        texture.format = tx_format;
        textures.push_back(texture);
    }
    if (AI_SUCCESS == aiGetMaterialColor(material, AI_MATKEY_COLOR_AMBIENT, &ambient_col))
    {
        Mesh::Texture texture;
        texture.color = glm::vec4(ambient_col.r, ambient_col.g, ambient_col.b, ambient_col.a);
        texture.type = TEXTYPEenum::AMBIENT;
        texture.format = tx_format;
        textures.push_back(texture);
    }
    // if (AI_SUCCESS == aiGetMaterialColor(material, AI_MATKEY_COLOR_SPECULAR, &specular_col))
    //{
    //	Mesh::Texture texture;
    //	texture.color = glm::vec4(specular_col.r, specular_col.g, specular_col.b, specular_col.a);
    //	texture.type = TEXTYPEenum::SPECULAR;
    //	texture.format = tx_format;
    //	textures.push_back(texture);
    // }
    // if (AI_SUCCESS == aiGetMaterialColor(material, AI_MATKEY_COLOR_EMISSIVE, &emissive_col))
    //{
    //	Mesh::Texture texture;
    //	texture.color = glm::vec4(emissive_col.r, emissive_col.g, emissive_col.b, emissive_col.a);
    //	texture.type = TEXTYPEenum::EMISSIVE;
    //	texture.format = tx_format;
    //	textures.push_back(texture);
    // }

    return textures;
}
