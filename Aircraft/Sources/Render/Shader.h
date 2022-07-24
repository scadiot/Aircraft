#pragma once

#include <string>
#include <GL/glew.h>

enum class ShaderType {
	Vertex, Fragment
};

class Shader {
public:
	Shader(std::string pFile, ShaderType pType);
	GLuint id();
protected:
	GLuint mShaderId;
};