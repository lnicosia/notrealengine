#ifndef _TEXTURE_LOADER_CLASS_H_
# define _TEXTURE_LOADER_CLASS_H_

#include "Object/Texture.class.hpp"
#include <map>
#include <memory>
#include <filesystem>

namespace notrealengine
{
	class TextureLoader
	{
	public:
		//TextureLoader() noexcept;
		//~TextureLoader();

		static std::shared_ptr<Texture>	loadTexture(std::string path, std::string type);


	private:
		static std::map<std::string, std::shared_ptr<Texture>>	textures;

	};
}

#endif