#include "Texture.h"
//#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

Texture::Texture(std::string pPath)
{
    mPath = pPath;
    mLoadingState = LoadingState::WaitForImageLoading;
    mTextureId = 0;
}

void Texture::loadImageFile()
{
    mLoadingState = LoadingState::ImageLoading;
    stbi_set_flip_vertically_on_load(true);
    mImageData = stbi_load(mPath.c_str(), &mWidth, &mHeight, &mNrChannels, 0);
    mLoadingState = LoadingState::WaitForGlLoading;
}

void Texture::loadGlTexture()
{
    mLoadingState = LoadingState::GlLoading;
    glGenTextures(1, &mTextureId);
    // glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, mTextureId);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, mWidth, mHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, mImageData);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(mImageData);

    mLoadingState = LoadingState::LoadingDone;
}

GLuint Texture::id()
{
	return mTextureId;
}

void Texture::bind()
{
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, mTextureId);
}

bool Texture::isLoading()
{
    return mLoadingState == LoadingState::LoadingDone;
}

Texture::LoadingState Texture::getLoadingState()
{
    return mLoadingState;
}