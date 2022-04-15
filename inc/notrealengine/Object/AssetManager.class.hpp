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
			for (const auto& pair : this->assets)
			{
				std::shared_ptr<Asset> asset = pair.second;
				if (std::filesystem::exists(std::filesystem::path(path))
					&& std::filesystem::exists(asset->getPath())
					&& std::filesystem::equivalent(std::filesystem::path(asset->getPath()),
						std::filesystem::path(path)))
				{
					std::shared_ptr<T> tmp = dynamic_pointer_cast<T>(this->assets[asset->getId()]);
					//	If the cast successed, we found our asset. Return it instead of
					//	loading it again
					if (tmp)
						return tmp;
					//	If not, it can be an asset with the same path but of different type.
					//	For example, embedded textures or animations share the same path
					//	with their object
				}
			}

			std::shared_ptr<T> ptr(new T(path, std::forward<Args>(args)...));
			assets.emplace(std::make_pair(ptr->getId(), ptr));
			return ptr;
		}

		template <typename T>
		std::shared_ptr<T> getAsset(uint32_t id)
		{
			return assets[id];
		}

		template <typename T>
		std::vector<std::shared_ptr<T>> getAssetsOfType()
		{
			std::vector<std::shared_ptr<T>> res;
			for (const auto& pair: this->assets)
			{
				std::shared_ptr<T> asset = dynamic_pointer_cast<T>(this->assets[pair.second->getId()]);
				if (asset)
					res.push_back(asset);
			}
			return res;
		}

		void
			printContent(void) const;

		/**	Clear contents
		*/
		void
			clear(void);

	private:
		AssetManager();

		std::map<uint32_t, std::shared_ptr<Asset>>	assets;
	};
}
#endif // !_ASSET_MANAGER_CLASS_H_
