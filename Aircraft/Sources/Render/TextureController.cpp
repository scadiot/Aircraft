#include "TextureController.h"

TextureController::TextureController()
{
	mStopLoadingThread = false;
	mLoadingThread = new std::thread(&TextureController::loadingThread, this);
}

TextureController::~TextureController()
{
	mLoadingThread->join();
	delete mLoadingThread;
}

Texture* TextureController::loadTexture(std::string mName)
{
	auto it = mTexturesByName.find(mName);
	if (it != mTexturesByName.end()) {
		return it->second;
	}

	std::string path = "C:\\TMP\\aircraft\\tiles\\" + mName;// "Contents\\" + mName;
	Texture* texture = new Texture(path);
	std::lock_guard<std::mutex> guard(mTexturesMutex);
	mTextures.push_back(texture);
	mTexturesByName.insert(std::pair<std::string, Texture* >(mName, texture));
	mLoadingTextures.push_back(texture);

	return texture;
}

void TextureController::loadingThread()
{
	while (!mStopLoadingThread) {
		std::this_thread::sleep_for(std::chrono::milliseconds(50));
		uint32_t index = 0;
		mTexturesMutex.lock();
		bool isLast = mLoadingTextures.empty();
		mTexturesMutex.unlock();
		while (!isLast) {
			Texture* texture = nullptr;
			mTexturesMutex.lock();
			if (index < mLoadingTextures.size()) {
				texture = mLoadingTextures.at(index);
			}
			mTexturesMutex.unlock();
			if (texture != nullptr && texture->getLoadingState() == Texture::LoadingState::WaitForImageLoading) {
				texture->loadImageFile();
			}
			index++;
			mTexturesMutex.lock();
			isLast = !(index < mLoadingTextures.size());
			mTexturesMutex.unlock();
		}
	}
}

void TextureController::update()
{
	uint32_t index = 0;
	mTexturesMutex.lock();
	bool isLast = mLoadingTextures.empty();
	mTexturesMutex.unlock();
	while (!isLast) {
		Texture* texture = nullptr;
		mTexturesMutex.lock();
		if (index < mLoadingTextures.size()) {
			texture = mLoadingTextures.at(index);
		}
		mTexturesMutex.unlock();
		if (texture != nullptr && texture->getLoadingState() == Texture::LoadingState::WaitForGlLoading) {
			texture->loadGlTexture();
			mTexturesMutex.lock();
			mLoadingTextures.erase(mLoadingTextures.begin() + index);
			mTexturesMutex.unlock();
			index--;
		}
		index++;
		mTexturesMutex.lock();
		isLast = !(index < mLoadingTextures.size());
		mTexturesMutex.unlock();
	}
}