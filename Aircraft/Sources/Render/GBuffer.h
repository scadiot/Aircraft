#pragma once

#include <GL/glew.h>

class Texture;

class GBuffer {
public:
	GBuffer(GLuint pWidth, GLuint pHeight);
	GLuint id();
	void bind();
	void bindTexture();

	GLuint texturePositionId();
protected:
	GLuint mWidth;
	GLuint mHeight;

	GLuint mFramebufferId;

	GLuint mTexturePositionId;
	GLuint mTextureNormalId;
	GLuint mTextureAlbedoSpecId;
};