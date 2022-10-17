#pragma once

#include <string>
#include <GL/glew.h>

class Texture {
public:
	Texture(std::string pPath);
	GLuint id();
	void bind();
	bool isLoading();

	enum LoadingState {
		WaitForImageLoading,
		ImageLoading,
		WaitForGlLoading,
		GlLoading,
		LoadingDone
	};
protected:
	void loadImageFile();
	void loadGlTexture();
	LoadingState getLoadingState();

	GLuint mTextureId;
	LoadingState mLoadingState;

	std::string mPath;
	unsigned char* mImageData;
	int32_t mWidth, mHeight, mNrChannels;


	friend class TextureController;
};