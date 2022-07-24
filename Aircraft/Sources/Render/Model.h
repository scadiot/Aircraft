#pragma once

#include <GL/glew.h>
#include <string>
#include <vector>

class Mesh;

class Model {
public:
	Model(const std::string& pFile);
	const std::vector<Mesh*>& meshes();
protected:
	std::vector<Mesh*> mMeshes;
};