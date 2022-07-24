#pragma once

#include <GL/glew.h>

class Framebuffer {
public:
	Framebuffer(GLuint pWidth, GLuint pHeight);
	GLuint id();
	GLuint textureId();
	void bind();
	void bindTexture();
protected:
	GLuint mWidth;
	GLuint mHeight;
	GLuint mFramebufferId;
	GLuint mTextureId;
};