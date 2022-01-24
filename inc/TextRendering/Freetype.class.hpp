
#ifndef  __Freetype_CLASS_H__
# define __Freetype_CLASS_H__

#include <ft2build.h>
#include FT_FREETYPE_H

namespace notrealengine
{

	class Freetype {

	public:
		static void Init();
		static void Quit();

		static FT_Library	getFT();

	private:
		Freetype() = default;;

		static bool initialized;

		static FT_Library	ft;
	};
} // namespace notrealengine

#endif

