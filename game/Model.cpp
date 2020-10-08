#include "Model.h"

Model::Model(const std::string& path, bool gamma) :
	gammaCorrection(gamma)
{
	loadModel(path);
}

void Model::Draw(Shader& shader)
{
	for (size_t i = 0; i < meshes.size(); i++)
	{
		meshes[i].Draw(shader);
	}
}

void Model::loadModel(const std::string& path)
{
	Assimp::Importer importer;

	/*
	* Read the file. 
	* Assimp's interface defines each mesh as having an array of faces, where each face
	* is represented by a single primitive. The aiProcess_Triangulate flag defines the 
	* primitives as triangles.
	*/
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

	if (!scene || !scene->mRootNode || (scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE))
	{
		std::cout << "ERROR::MODEL::LOAD_MODEL::ASSIMP" << std::endl;
		std::cout << importer.GetErrorString() << std::endl;
		return;
	}

	directory = path.substr(0, path.find_last_of('/'));

	processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode* node, const aiScene* scene)
{
	// First process all the node's meshes (if any)
	for (std::size_t i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(processMesh(mesh, scene));
	}

	// Then process all of the node's children
	for (size_t i = 0; i < node->mNumChildren; i++)
	{
		processNode(node->mChildren[i], scene);
	}
}

Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene)
{
	std::vector<Mesh::Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Mesh::Texture> textures;

	/*
	* Process the vertices.
	* Construct the position, normal and texture vectors.
	*/
	for (std::size_t i = 0; i < mesh->mNumVertices; i++)
	{
		Mesh::Vertex vertex;

		// Construct the position vector for this vertex
		glm::vec3 positionVector;
		positionVector.x = mesh->mVertices[i].x;
		positionVector.y = mesh->mVertices[i].y;
		positionVector.z = mesh->mVertices[i].z;
		vertex.position = positionVector;

		// Construct the normal vector for this vertex
		glm::vec3 normalVector;
		normalVector.x = mesh->mNormals[i].x;
		normalVector.y = mesh->mNormals[i].y;
		normalVector.z = mesh->mNormals[i].z;
		vertex.normal = normalVector;

		// If the texture coordinates exist, construct the texture vector for this vertex
		// The index 0 is because Assimp allows for up to 8 differrent sets of texture coordinates
		if (mesh->mTextureCoords[0])
		{
			// Texture coordinates
			glm::vec2 textureVector;
			textureVector.x = mesh->mTextureCoords[0][i].x;
			textureVector.y = mesh->mTextureCoords[0][i].y;
			vertex.textureCoordinates = textureVector;

			// Tangent vector to the vertex
			if (mesh->mTangents)
			{
				glm::vec3 tangentVector;
				tangentVector.x = mesh->mTangents[i].x;
				tangentVector.y = mesh->mTangents[i].y;
				tangentVector.z = mesh->mTangents[i].z;
				vertex.tangent = tangentVector;
			}

			// Bitangent vector to the vertex (orthogonal to tangent and normal)
			if (mesh->mBitangents)
			{
				glm::vec3 biTangentVector;
				biTangentVector.x = mesh->mBitangents[i].x;
				biTangentVector.y = mesh->mBitangents[i].y;
				biTangentVector.z = mesh->mBitangents[i].z;
				vertex.biTangent = biTangentVector;
			}
		}
		else
		{
			vertex.textureCoordinates = glm::vec2(0.0f, 0.0f);
		}

		vertices.push_back(vertex);
	}
	
	/*
	* Process the mesh faces i.e. the primitives.
	* In this case the primitives are triangles (due to the aiProcess_Triangulate flag).
	* For further explanation look at the Model::loadModel method.
	*/
	for (std::size_t i = 0; i < mesh->mNumFaces; i++)
	{
		// Get a face from the mesh
		aiFace face = mesh->mFaces[i];

		// Loop through the indices of this face
		for (std::size_t j = 0; j < face.mNumIndices; j++)
		{
			indices.push_back(face.mIndices[j]);
		}
	}

	if (mesh->mMaterialIndex >= 0)
	{
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

		// 1. Load the diffuse maps
		std::vector<Mesh::Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, TEXTYPEenum::DIFFUSE);
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end()); // Insert the diffuse map to the end of textures struct

		// 2. Load the specular maps
		std::vector<Mesh::Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, TEXTYPEenum::SPECULAR);
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end()); // Insert the specular map to the end of textures struct

		// 3. Load the normal maps
		std::vector<Mesh::Texture> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, TEXTYPEenum::NORMAL);
		textures.insert(textures.end(), normalMaps.begin(), normalMaps.end()); // Insert the normal map to the end of textures struct

		// 4. Load the height maps
		std::vector<Mesh::Texture> heightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, TEXTYPEenum::HEIGHT);
		textures.insert(textures.end(), heightMaps.begin(), heightMaps.end()); // Insert the height map to the end of textures struct
	}

	return Mesh(vertices, indices, textures);
}

std::vector<Mesh::Texture> Model::loadMaterialTextures(aiMaterial* material, aiTextureType aiType, TEXTYPEenum txType)
{
	std::vector<Mesh::Texture> textures;
	for (std::size_t i = 0; i < material->GetTextureCount(aiType); i++)
	{
		aiString path;
		material->GetTexture(aiType, (unsigned int)i, &path);
		// Flag for checking if the texture has been loaded already
		bool alreadyLoaded = false;

		for (std::size_t j = 0; j < texturesLoaded.size(); j++)
		{
			if (std::strcmp(texturesLoaded.at(j).path.c_str(), path.C_Str()) == 0)
			{
				textures.push_back(texturesLoaded.at(j));
				alreadyLoaded = true;
				break;
			}
		}

		if (!alreadyLoaded)
		{
			Mesh::Texture texture;
			texture.id = Mesh::LoadMaterialTextureFromFile((std::string&)(path.C_Str()), directory, gammaCorrection);
			texture.type = txType;
			texture.path = path.C_Str();
			textures.push_back(texture);
			texturesLoaded.push_back(texture);
		}
	}

	return textures;
}