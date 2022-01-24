#ifndef _GLFONT_CLASS_H_
# define _GLFONT_CLASS_H_

#include "TextRendering/GLCharacter.class.hpp"
#include "GLShaderProgram.class.hpp"

#include <map>
#include <string>

namespace notrealengine
{
	class GLFont
	{
	public:
		GLFont() = delete;
		explicit GLFont(const std::string& path);
		GLFont(GLFont const& ref) = delete;
		GLFont(GLFont&& ref) noexcept;
		GLFont& operator=(GLFont const& font) = delete;
		GLFont& operator=(GLFont&& font) noexcept;
		~GLFont();

		void	RenderText(GLShaderProgram* shader, const std::string& text,
			mft::vec2 pos, float scale, mft::vec3 color);

	private:
		std::map<char, GLCharacter*>	characters;

		unsigned int	VAO{}, VBO{};
	};
} // namespace notrealengine

#endif