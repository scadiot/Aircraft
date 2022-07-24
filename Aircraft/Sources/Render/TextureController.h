#pragma once

#include "Texture.h"
#include <vector>

class TextureController {
public:
	TextureController();
	Texture* loadTexture(std::string mName);
protected:
	std::vector<Texture*> mTextures;
};