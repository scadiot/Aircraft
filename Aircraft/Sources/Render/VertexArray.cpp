#include "VertexArray.h"
#include "RenderController.h"


VertexArray::VertexArray()
{
    mVbo = 0;
    mEbo = 0;
}

void VertexArray::setVertices(const VertexNormalUv* vertices, unsigned int pVerticesCount)
{
    mFacesCount = pVerticesCount / 2;

    if (mVbo == 0) {
        glGenBuffers(1, &mVbo);
        glBindBuffer(GL_ARRAY_BUFFER, mVbo);
        glBufferData(GL_ARRAY_BUFFER, pVerticesCount * sizeof(VertexNormalUv), nullptr, GL_STATIC_DRAW);

        glGenVertexArrays(1, &mVertexArrayId);
    }

    glBindVertexArray(mVertexArrayId);
    glBindBuffer(GL_ARRAY_BUFFER, mVbo);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexNormalUv), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexNormalUv), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // texture coord attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VertexNormalUv), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, mVbo);
    void* ptr = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
    memcpy(ptr, vertices, pVerticesCount * sizeof(VertexNormalUv));
    glUnmapBuffer(GL_ARRAY_BUFFER);
}

void VertexArray::setIndices(const unsigned int* indices, unsigned int pIndicesCount)
{
    mFacesCount = pIndicesCount / 3;

    glBindBuffer(GL_ARRAY_BUFFER, mVertexArrayId);
    if (mEbo == 0) {
        glGenVertexArrays(1, &mEbo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEbo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, pIndicesCount * sizeof(unsigned int), nullptr, GL_STATIC_DRAW);
    }
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEbo);
    void* ptr = glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_WRITE_ONLY);
    memcpy(ptr, indices, pIndicesCount * sizeof(unsigned int));
    glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
}

GLuint VertexArray::id()
{
    return mVertexArrayId;
}

void VertexArray::bind()
{
    glBindVertexArray(mVertexArrayId);
}

GLuint VertexArray::facesCount()
{
    return mFacesCount;
}