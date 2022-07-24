#include "Model.h"
#include "Mesh.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

Model::Model(const std::string& pFile)
{
    Assimp::Importer importer;

    const aiScene* scene = importer.ReadFile(pFile,
        aiProcess_CalcTangentSpace |
        aiProcess_Triangulate |
        aiProcess_JoinIdenticalVertices |
        aiProcess_SortByPType);

    if (!scene)
    {
        throw "Model loading error";
    }

    for (int i = 0; i < scene->mNumMeshes; ++i) 
    {
        const C_STRUCT aiMesh* aiMesh = scene->mMeshes[i];
        Mesh* mesh = new Mesh(aiMesh);

        //aiMaterial* material = scene->mMaterials[aiMesh->mMaterialIndex];
        //aiString *texturePath = 0;
        //material->GetTexture(aiTextureType::aiTextureType_DIFFUSE, 0, texturePath);

        mMeshes.push_back(mesh);
    }
}

const std::vector<Mesh*>& Model::meshes()
{
    return mMeshes;
}