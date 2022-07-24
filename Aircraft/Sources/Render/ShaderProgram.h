#pragma once

#include <string>
#include <GL/glew.h>
#include <glm/glm.hpp>

class RenderController;
class Shader;

class ShaderProgram {
public:
	ShaderProgram(RenderController* pRenderController, std::string pVertexPath, std::string pFragmentPath);
	GLuint id();

	void setMvpMatrix(const glm::mat4& pMatrix);
	void setModelMatrix(const glm::mat4& pMatrix);
	virtual void use();
protected:
	RenderController* mRenderController;
	GLuint mShaderProgramId;
	Shader* mVertexShader;
	Shader* mFragmentShader;

	GLuint mMvpMatrixID;
	GLuint mModelMatrixID;
};