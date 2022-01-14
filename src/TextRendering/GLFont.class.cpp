#include "TextRendering/GLFont.class.hpp"
#include "TextRendering/Freetype.class.hpp"
#include "GL.hpp"

namespace notrealengine
{
	GLFont::GLFont(const std::string& path): Asset(path)
	{
		std::cout << "Loading font '" << path << "'..." << std::endl;
		Freetype::Init();

		FT_Library const& ft = Freetype::getFT();
		FT_Face	face;
		if (FT_New_Face(ft, path.c_str(), 0, &face))
		{
			std::cerr << "Could not init font " << path << std::endl;
		}

		FT_Set_Pixel_Sizes(face, 0, 48);

		if (FT_Load_Char(face, 'X', FT_LOAD_RENDER))
		{
			std::cerr << "Could not load Glyph" << std::endl;
			return;
		}

		GLCallThrow(glPixelStorei, GL_UNPACK_ALIGNMENT, 1);

		for (unsigned char c = 0; c < 128; c++)
		{
			characters.insert(std::make_pair(c, new GLCharacter(face, c)));
		}

		FT_Done_Face(face);

		GLCallThrow(glGenVertexArrays, 1, &VAO);
		GLCallThrow(glGenBuffers, 1, &VBO);
		GLCallThrow(glBindVertexArray, VAO);

		GLCallThrow(glBindBuffer, GL_ARRAY_BUFFER, VBO);
		GLCallThrow(glBufferData, GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
		
		GLCallThrow(glEnableVertexAttribArray, 0);
		GLCallThrow(glVertexAttribPointer, 0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);

		GLCallThrow(glBindBuffer, GL_ARRAY_BUFFER, 0);
		GLCallThrow(glBindVertexArray, 0);

	}

	GLFont::GLFont(GLFont&& ref) noexcept
		: Asset(ref.getPath()), characters(std::move(ref.characters)),
		VAO(std::exchange(ref.VAO, 0)),
		VBO(std::exchange(ref.VBO, 0))
	{

	}

	GLFont::~GLFont()
	{
		GLCallThrow(glDeleteVertexArrays, 1, &VAO);
		GLCallThrow(glDeleteBuffers, 1, &VBO);
	}

	const std::string	GLFont::getAssetType() const
	{
		return std::string("GLFont");
	}

	GLFont& GLFont::operator=(GLFont&& font) noexcept
	{
		this->characters = std::move(font.characters);
		this->VAO = std::exchange(font.VAO, 0);
		this->VBO = std::exchange(font.VBO, 0);
		return *this;
	}

	void	GLFont::RenderText(GLShaderProgram* shader, std::string text,
		mft::vec2 pos, float scale, mft::vec3 color)
	{
		GLCallThrow(glUseProgram, shader->programID);
		GLCallThrow(glUniform3f,
			GLCallThrow(glGetUniformLocation, shader->programID, "textColor"), color.x, color.y, color.z);
		GLCallThrow(glActiveTexture, GL_TEXTURE0);
		GLCallThrow(glBindVertexArray, VAO);

		for (auto c : text)
		{
			GLCharacter* ch = characters[c];

			float	xpos = pos.x + ch->getBearing().x * scale;
			float	ypos = pos.y - (ch->getSize().y - ch->getBearing().y) * scale;

			float	w = ch->getSize().x * scale;
			float	h = ch->getSize().y * scale;

			float	vertices[6][4] =
			{
				{xpos,		ypos + h,	0.0f, 0.0f},
				{xpos,		ypos,		0.0f, 1.0f},
				{xpos + w,	ypos,		1.0f, 1.0f},

				{xpos,		ypos + h,	0.0f, 0.0f},
				{xpos + w,	ypos,		1.0f, 1.0f},
				{xpos + w,	ypos + h,	1.0f, 0.0f}
			};

			GLCallThrow(glBindTexture, GL_TEXTURE_2D, ch->getId());

			GLCallThrow(glBindBuffer, GL_ARRAY_BUFFER, VBO);
			GLCallThrow(glBufferSubData, GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
			GLCallThrow(glBindBuffer, GL_ARRAY_BUFFER, 0);

			GLCallThrow(glDrawArrays, GL_TRIANGLES, 0, 6);

			pos.x += (ch->getNext() >> 6) * scale;
		}

		GLCallThrow(glBindVertexArray, 0);
		GLCallThrow(glBindTexture, GL_TEXTURE_2D, 0);
	}
}