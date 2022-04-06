#include "Object/AssetManager.class.hpp"

namespace notrealengine
{
	AssetManager::AssetManager(): assets()
	{
	}

	AssetManager::~AssetManager()
	{
	}

	AssetManager& AssetManager::getInstance()
	{
		static AssetManager	instance;
		return instance;
	}

	void	AssetManager::printContent() const
	{
		if (assets.size() == 0)
			std::cout << "Empty" << std::endl;
		for (const auto& asset : assets)
		{
			std::cout << (*asset.second);
		}
	}
}