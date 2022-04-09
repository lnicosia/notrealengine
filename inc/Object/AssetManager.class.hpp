#ifndef _ASSET_MANAGER_CLASS_H_
#define _ASSET_MANAGER_CLASS_H_

#include "Object/Asset.class.hpp"

#include <map>
#include <iostream>

namespace notrealengine
{
	class AssetManager
	{
	public:
		AssetManager(const AssetManager& ref) = delete;
		AssetManager(const AssetManager&& ref) = delete;
		AssetManager&	operator=(const AssetManager& ref) = delete;
		AssetManager&	operator=(const AssetManager&& ref) = delete;
		~AssetManager();

		static AssetManager&
			getInstance();

		template <typename T, typename ... Args>
		std::shared_ptr<T> loadAsset(const std::string& path, Args... args)
		{
			for (const auto& pair : assets)
			{
				std::shared_ptr<Asset> asset = pair.second;
				if (std::filesystem::exists(std::filesystem::path(path))
					&& std::filesystem::exists(asset->getPath())
					&& std::filesystem::equivalent(std::filesystem::path(asset->getPath()),
						std::filesystem::path(path)))
				{
					std::shared_ptr<T> tmp = dynamic_pointer_cast<T>(assets[asset->getId()]);
					if (tmp)
						return tmp;
				}
			}

			std::shared_ptr<T> ptr(new T(path, std::forward<Args>(args)...));
			assets.emplace(std::make_pair(ptr->getId(), ptr));
			return ptr;
		}

		template <typename T, typename ... Args>
		std::shared_ptr<T> getAsset(uint32_t id)
		{
			return assets[id];
		}

		void
			printContent(void) const;

	private:
		AssetManager();

		std::map<uint32_t, std::shared_ptr<Asset>>	assets;
	};
}
#endif // !_ASSET_MANAGER_CLASS_H_
