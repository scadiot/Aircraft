#include "Mesh.h"
#include "VertexArray.h"

#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags

#include <vector>

Mesh::Mesh(const aiMesh* mesh)
{
    std::vector<VertexNormalUv> verticesData;// (mesh->mNumFaces * 3);
    std::vector<unsigned int> indicesData;// (mesh->mNumFaces * 3);
    float val = 0;
    for (unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        const C_STRUCT aiFace* aiFace = &mesh->mFaces[i];
        for (unsigned int j = 0; j < aiFace->mNumIndices; j++) {

            int index = aiFace->mIndices[j];
            VertexNormalUv vertice;
            vertice.mPosition[0] = mesh->mVertices[index].x;
            vertice.mPosition[1] = mesh->mVertices[index].y;
            vertice.mPosition[2] = mesh->mVertices[index].z;
            val = val < vertice.mPosition[2] ? vertice.mPosition[2] : val;
            vertice.mNormal[0] = mesh->mNormals[index].x;
            vertice.mNormal[1] = mesh->mNormals[index].y;
            vertice.mNormal[2] = mesh->mNormals[index].z;

            vertice.mUv[0] = mesh->mTextureCoords[0][index].x;
            vertice.mUv[1] = mesh->mTextureCoords[0][index].y;

            verticesData.push_back(vertice);
            indicesData.push_back(i * 3 + j);
        }
    }

    mVertexArray = new VertexArray(&verticesData[0], (unsigned int)verticesData.size());
    mVertexArray->setIndices(&indicesData[0], (unsigned int)indicesData.size());
}

VertexArray* Mesh::vertexArray()
{
    return mVertexArray;
}

void Mesh::setTexture(Texture* pTexture)
{
    mTexture = pTexture;
}