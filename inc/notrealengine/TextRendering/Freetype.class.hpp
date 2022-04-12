
#ifndef  __Freetype_CLASS_H__
# define __Freetype_CLASS_H__

#include <ft2build.h>
#include FT_FREETYPE_H

namespace notrealengine
{

	class Freetype {

	public:
		static void Init(void);
		static void Quit(void);

		static FT_Library	getFT();

	private:
		Freetype(void) {};

		static bool initialized;

		static FT_Library	ft;
	};
}

#endif

