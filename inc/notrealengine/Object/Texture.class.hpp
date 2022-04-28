#ifndef _TEXTURE_CLASS_H_
# define _TEXTURE_CLASS_H_

#include "Object/Asset.class.hpp"
#include "GLShaderProgram.class.hpp"
#include "mft/mft.hpp"

#include <string>
#include <set>

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
		const std::string&
			getType() const;
		const mft::vec2i&
			getSize() const;
		const GLShaderProgram*
			getShader() const;
		const unsigned int
			getShaderID() const;

		//	Asset class

		const std::string
			getAssetType() const override;

		//	Setters

		void
			setType(const std::string& type);
		void
			setShader(GLShaderProgram* shader);

		//	Draw

		/**	Draw the texture at the giving pos, with the given size, color and rotation
		**	and either a specific shader pointer of the texture's one
		*/
		void
			draw(mft::vec2i pos, mft::vec2i size, float rotation, mft::vec4 color,
				GLShaderProgram* shader = nullptr) const;

	private:
		unsigned int	glId, VAO, VBO;
		GLShaderProgram* shader;
		std::string		type;
		mft::vec2i		size;

	};
}

#endif
