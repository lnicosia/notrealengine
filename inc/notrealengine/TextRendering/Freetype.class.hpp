
#ifndef  _Freetype_CLASS_H_
# define _Freetype_CLASS_H_

#ifdef USING_EXTERNAL_LIBS
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
#endif // !USING_EXTERNAL_LIBS

#endif // ! _Freetype_CLASS_H_