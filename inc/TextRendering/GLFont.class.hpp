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

		/**	Loads a BitmapFont generated image
		*/
		void
			LoadBFF(const std::string& path);

		/**	Draw the text at the giving pos, with the given scale and color
		**	and either a specific shader pointer of the font's one
		**	- Not const to get characters from the map
		*/
		void
			RenderText(std::string text, mft::vec2i pos, float scale, const mft::vec4& color,
				GLShaderProgram* shader = nullptr);

		//	Getters

		const std::map<char, GLCharacter*>&
			getCharacters() const;

		const GLCharacter*
			getCharacter(char c);

		virtual const std::string
			getAssetType() const;

		const GLShaderProgram*
			getShader() const;
		const unsigned int
			getShaderID() const;

		//	Setters

		void	setShader(GLShaderProgram* shader);

	private:
		std::map<char, GLCharacter*>	characters;

		unsigned int	VAO, VBO;

		GLShaderProgram* shader;

#ifdef USING_EXTERNAL_LIBS
#else
		mft::vec2i		imgSize;
		mft::vec2i		cellSize;
		mft::vec2			factor;
		unsigned int	charsPerLine;
		unsigned int 	glId;
		char					widths[256];
		char					firstChar;
		#define WIDTH_DATA_OFFSET  20 // Offset to width data with BFF file
		#define MAP_DATA_OFFSET   276 // Offset to texture image data with BFF fil
#endif
	};
}

#endif
