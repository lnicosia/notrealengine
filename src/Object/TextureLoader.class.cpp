#include "Object/TextureLoader.class.hpp"

#include <iostream>
#include <utility>

namespace notrealengine
{
	/*TextureLoader::TextureLoader() noexcept
	{

	}

	TextureLoader::~TextureLoader()
	{

	}*/

	std::map<std::string, std::shared_ptr<Texture>> TextureLoader::textures =
		std::map<std::string, std::shared_ptr<Texture>>();

	std::shared_ptr<Texture>	TextureLoader::loadTexture(const std::string& path, std::string type)
	{
		//	Manual check
		//	Verifiy if the map does not already
		//	contain an equivalent path
		for (const auto& pair : textures)
		{
			if (std::filesystem::equivalent(std::filesystem::path(pair.first),
				std::filesystem::path(path)))
			{
				//std::cout << "Texture '" << path << "' already exists, returning it" << std::endl;
				return textures[pair.first];
			}
		}

		std::shared_ptr<Texture>	ptr(new Texture(path, std::move(type)));
		textures.emplace(std::make_pair(path, ptr));
		//std::cout << "Texture '" << path << "' shared_ptr created" << std::endl;
		return ptr;
	}
} // namespace notrealengine