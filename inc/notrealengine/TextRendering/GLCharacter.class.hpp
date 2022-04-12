#ifndef _GLCHARACTER_CLASS_H_
# define _GLCHARACTER_CLASS_H_

#include "mft/mft.hpp"

#include <ft2build.h>
#include FT_FREETYPE_H

namespace notrealengine
{
	struct GLCharacter
	{
	public:
		GLCharacter() = delete;
		GLCharacter(GLCharacter const& ref) = delete;
		GLCharacter(GLCharacter&& ref) noexcept;
		GLCharacter(FT_Face face, char c);
		GLCharacter& operator=(GLCharacter const& ref) = delete;
		GLCharacter& operator=(GLCharacter && c) noexcept;
		~GLCharacter();

		unsigned int const& getId() const;
		mft::vec2i const& getSize() const;
		mft::vec2i const& getBearing() const;
		unsigned int const& getNext() const;

	private:
		unsigned int	glId;
		mft::vec2i		size;
		mft::vec2i		bearing;
		unsigned int	next;

		void	setup(FT_Face face, char c);
	};
}

#endif