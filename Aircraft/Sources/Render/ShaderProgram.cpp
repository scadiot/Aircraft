#include "RenderController.h"
#include "ShaderProgram.h"
#include "Shader.h"
#include "ShaderController.h"

ShaderProgram::ShaderProgram(RenderController* pRenderController, std::string pVertexPath, std::string pFragmentPath)
{
	mRenderController = pRenderController;

	mVertexShader = mRenderController->shaderController()->loadShader(pVertexPath, ShaderType::Vertex);
	mFragmentShader = mRenderController->shaderController()->loadShader(pFragmentPath, ShaderType::Fragment);

	mShaderProgramId = glCreateProgram();
	glAttachShader(mShaderProgramId, mVertexShader->id());
	glAttachShader(mShaderProgramId, mFragmentShader->id());
	glLinkProgram(mShaderProgramId);

	GLchar    buff[1024];
	GLsizei     length;
	glGetProgramInfoLog(mShaderProgramId, 1024, &length, buff);

	mMvpMatrixID = glGetUniformLocation(mShaderProgramId, "mvpMatrix");
	mModelMatrixID = glGetUniformLocation(mShaderProgramId, "modelMatrix");
}

GLuint ShaderProgram::id()
{
	return mShaderProgramId;
}

void ShaderProgram::setMvpMatrix(const glm::mat4& pMatrix)
{
	glUniformMatrix4fv(mMvpMatrixID, 1, GL_FALSE, &pMatrix[0][0]);
}

void ShaderProgram::setModelMatrix(const glm::mat4& pMatrix)
{
	glUniformMatrix4fv(mModelMatrixID, 1, GL_FALSE, &pMatrix[0][0]);
}

void ShaderProgram::use()
{
	glUseProgram(mShaderProgramId);
}