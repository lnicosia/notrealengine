#ifndef _TEXTURE_LOADER_CLASS_H_
# define _TEXTURE_LOADER_CLASS_H_

#include "Object/Texture.class.hpp"
#include <filesystem>
#include <map>
#include <memory>

namespace notrealengine
{
	class TextureLoader
	{
	public:
		//TextureLoader() noexcept;
		//~TextureLoader();

		static std::shared_ptr<Texture>	loadTexture(const std::string& path, std::string type);


	private:
		static std::map<std::string, std::shared_ptr<Texture>>	textures;

	};
} // namespace notrealengine

#endif