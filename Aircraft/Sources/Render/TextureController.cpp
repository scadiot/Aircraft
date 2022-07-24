#include "TextureController.h"

TextureController::TextureController()
{

}

Texture* TextureController::loadTexture(std::string mName)
{
	std::string path = "C:\\TMP\\aircraft\\Content\\" + mName;
	Texture* texture = new Texture(path);
	mTextures.push_back(texture);

	return texture;
}
