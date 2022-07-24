#include "Shader.h"
#include <iostream>
#include <fstream>
#include <vector>

std::string loadFile(std::string path) {
	std::string result = "";
	std::string line;
	std::ifstream myfile(path);
	if (myfile.is_open())
	{
		while (getline(myfile, line))
		{
			result += line + '\n';
		}
		myfile.close();
	}
	return result;
}

Shader::Shader(std::string pFile, ShaderType pType)
{
	const std::string shaderCodeStr = loadFile(pFile);
	const char* shaderCode = shaderCodeStr.c_str();

	mShaderId = glCreateShader(pType == ShaderType::Vertex ? GL_VERTEX_SHADER : GL_FRAGMENT_SHADER);
	glShaderSource(mShaderId, 1, &shaderCode, NULL);
	glCompileShader(mShaderId);

	GLint success = 0;
	glGetShaderiv(mShaderId, GL_COMPILE_STATUS, &success);
	if (success == GL_FALSE) {
		GLint maxLength = 0;
		glGetShaderiv(mShaderId, GL_INFO_LOG_LENGTH, &maxLength);
		std::vector<char> errorLog(maxLength);
		glGetShaderInfoLog(mShaderId, maxLength, &maxLength, &errorLog[0]);
		std::cout << &errorLog[0] << std::endl;
		throw "error";
	}
}

GLuint Shader::id()
{
	return mShaderId;
}