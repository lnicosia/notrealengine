#ifndef _TEXTURE_CLASS_H_
# define _TEXTURE_CLASS_H_

#include "Object/Asset.class.hpp"

#include <string>
#include <set>

#include "mft/mft.hpp"
#include "GLShaderProgram.class.hpp"

namespace notrealengine
{
	class Texture: public Asset
	{
	public:
		Texture() = delete;
		Texture(const Texture& ref) = delete;
		Texture(Texture && ref) noexcept;
		/**	Load a texture directly from its path
		*/
		Texture(const std::string& path, const std::string& type);

		/**	Load a texture embedded inside another file
		*/
		Texture(const std::string& path, unsigned char *data, unsigned int size, const std::string& type);
		~Texture();
		Texture& operator=(const Texture& text) = delete;
		Texture& operator=(Texture && text) noexcept;

		//	Accessors

		const unsigned int&
			getGLId() const;
		virtual const std::string
			getAssetType() const;
		const std::string&
			getType() const;
		const mft::vec2i&
			getSize() const;

		//	Setters

		void	setType(const std::string& type);

		void
			draw(GLShaderProgram* shader, mft::vec2i pos,
				mft::vec2i size, float rotation, mft::vec3 color) const;

	private:
		unsigned int	glId, VAO, VBO;
		std::string		type;
		mft::vec2i		size;

	};
}

#endif
