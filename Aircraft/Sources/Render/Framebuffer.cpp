#include "Framebuffer.h"
#include <stddef.h>

Framebuffer::Framebuffer(GLuint pWidth, GLuint pHeight)
{
	mWidth = pWidth;
	mHeight = pHeight;

	glGenFramebuffers(1, &mFramebufferId);
	glBindFramebuffer(GL_FRAMEBUFFER, mFramebufferId);

	glGenTextures(1, &mTextureId);
	glBindTexture(GL_TEXTURE_2D, mTextureId);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, mWidth, mHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);

	// attach it to currently bound framebuffer object
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mTextureId, 0);

	unsigned int rbo;
	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, mWidth, mHeight);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);
}

GLuint Framebuffer::id()
{
	return mFramebufferId;
}

GLuint Framebuffer::textureId()
{
	return mTextureId;
}

void Framebuffer::bind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, mFramebufferId);
}

void Framebuffer::bindTexture()
{
	glBindTexture(GL_TEXTURE_2D, mTextureId);
}