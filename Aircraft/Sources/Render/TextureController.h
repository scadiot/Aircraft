#pragma once

#include "Texture.h"
#include <vector>
#include <unordered_map>
#include <string>
#include <thread>
#include <atomic>
#include <mutex>

class TextureController {
public:
	TextureController();
	~TextureController();
	Texture* loadTexture(std::string mName);
	void update();
protected:
	void loadingThread();
	std::vector<Texture*> mTextures;
	std::unordered_map<std::string, Texture*> mTexturesByName;
	std::vector<Texture*> mLoadingTextures;
	std::thread* mLoadingThread;
	std::atomic<bool> mStopLoadingThread;
	std::mutex mTexturesMutex;
};