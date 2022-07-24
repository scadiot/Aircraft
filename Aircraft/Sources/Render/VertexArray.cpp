#include "VertexArray.h"
#include "RenderController.h"

VertexArray::VertexArray(const VertexNormalUv* vertices, unsigned int pVerticesCount)
{
    mFacesCount = pVerticesCount / 2;

    GLuint vbo = 0;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, pVerticesCount * sizeof(VertexNormalUv), vertices, GL_STATIC_DRAW);
    //glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);

    glGenVertexArrays(1, &mVertexArrayId);
    glBindVertexArray(mVertexArrayId);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexNormalUv), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexNormalUv), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // texture coord attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VertexNormalUv), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
}

void VertexArray::setIndices(const unsigned int* indices, unsigned int pIndicesCount)
{
    mFacesCount = pIndicesCount / 3;

    glBindBuffer(GL_ARRAY_BUFFER, mVertexArrayId);

    GLuint ebo = 0;
    glGenVertexArrays(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, pIndicesCount * sizeof(unsigned int), indices, GL_STATIC_DRAW);
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