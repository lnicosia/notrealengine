#include "TextRendering/GLCharacter.class.hpp"
#include "GL.hpp"

#ifdef USING_EXTERNAL_LIBS
namespace notrealengine
{
	GLCharacter::GLCharacter(FT_Face face, char c) :
        glId(0), size(mft::vec2i(0, 0)), bearing(mft::vec2i(0, 0)), next(0)
	{
		setup(face, c);
	}

	GLCharacter::~GLCharacter()
	{
		GLCallThrow(glDeleteTextures, 1, &glId);
	}

	GLCharacter::GLCharacter(GLCharacter&& ref) noexcept
		: glId(std::exchange(ref.glId, 0)),
		size(std::move(ref.size)),
		bearing(std::move(ref.bearing)),
		next(std::exchange(ref.next, 0))
	{

	}

	GLCharacter& GLCharacter::operator=(GLCharacter&& c) noexcept
	{
		this->glId = std::exchange(c.glId, 0);
		this->size = std::move(c.size);
		this->bearing = std::move(c.bearing);
		this->next = std::exchange(c.next, 0);
		return *this;
	}

    void	GLCharacter::setup(FT_Face face, char c)
    {
		if (FT_Load_Char(face, c, FT_LOAD_RENDER))
		{
			std::cerr << "Freetype error: failed to load glyph " << c << std::endl;
			return;
		}

		GLCallThrow(glGenTextures, 1, &glId);
		GLCallThrow(glBindTexture, GL_TEXTURE_2D, glId);
		GLCallThrow(glTexImage2D,
			GL_TEXTURE_2D, 0, GL_RED,
			face->glyph->bitmap.width, face->glyph->bitmap.rows,
			0, GL_RED, GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer);

		GLCallThrow(glTexParameteri, GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		GLCallThrow(glTexParameteri, GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		GLCallThrow(glTexParameteri, GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		GLCallThrow(glTexParameteri, GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		size = mft::vec2i(face->glyph->bitmap.width, face->glyph->bitmap.rows);
		bearing = mft::vec2i(face->glyph->bitmap_left, face->glyph->bitmap_top);
		next = face->glyph->advance.x;
    }

	unsigned int const& GLCharacter::getId() const
	{
		return glId;
	}

	mft::vec2i const& GLCharacter::getSize() const
	{
		return size;
	}

	mft::vec2i const& GLCharacter::getBearing() const
	{
		return bearing;
	}

	unsigned int const& GLCharacter::getNext() const
	{
		return next;
	}
}
#endif