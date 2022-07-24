#pragma once

#include <string>
#include <GL/glew.h>

class Texture {
public:
	Texture(std::string pPath);
	GLuint id();
	void bind();
protected:
	GLuint mTextureId;
};