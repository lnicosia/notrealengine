#ifndef _GLFONT_CLASS_H_
# define _GLFONT_CLASS_H_

#include "TextRendering/GLCharacter.class.hpp"
#include "GLShaderProgram.class.hpp"
#include "Object/Asset.class.hpp"

#include <string>
#include <map>

namespace notrealengine
{
	class GLFont: public Asset
	{
	public:
		GLFont() = delete;
		GLFont(const std::string& path);
		GLFont(GLFont const& ref) = delete;
		GLFont(GLFont&& ref) noexcept;
		GLFont& operator=(GLFont const& font) = delete;
		GLFont& operator=(GLFont&& font) noexcept;
		~GLFont();

		void	RenderText(GLShaderProgram* shader, std::string text,
			mft::vec2i pos, float scale, mft::vec3 color);

		const std::map<char, GLCharacter*>&
			getCharacters() const;

		const GLCharacter*
			getCharacter(char c);

		virtual const std::string
			getAssetType() const;

	private:
		std::map<char, GLCharacter*>	characters;

		unsigned int	VAO, VBO;
	};
}

#endif