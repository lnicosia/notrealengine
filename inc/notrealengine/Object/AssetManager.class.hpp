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

		/**	For a given type, path and args, search the content of the manager
		**	for an existing and corresponding asset. If found, return it.
		**	If not, load it with its own load function
		*/
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

		/**	Same as loadAsset but does not load if not found.
		**	Use this function when you know for sure that the asset is already loaded
		*/
		template <typename T>
		std::shared_ptr<T> getAsset(const std::string& path)
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
					if (tmp)
						return tmp;
				}
			}
			return nullptr;
		}

		template <typename T>
		std::shared_ptr<T> getAsset(uint32_t id)
		{
			std::map<uint32_t, std::shared_ptr<Asset>>::iterator it =
				this->assets.find(id);
			if (it == this->assets.end())
				return nullptr;
			std::shared_ptr<T> tmp = dynamic_pointer_cast<T>(it->second);
			if (tmp)
				return tmp;
			return nullptr;
		}

		template <typename T>
		void addAsset(std::shared_ptr<T> asset)
		{
			if (this->assets.find(asset->getId()) == this->assets.end())
				assets.emplace(std::make_pair(asset->getId(), asset));
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
