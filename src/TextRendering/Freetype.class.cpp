#include "TextRendering/Freetype.class.hpp"

#include <iostream>

namespace notrealengine
{
	void	Freetype::Init()
	{
		if (!initialized) {
			if (FT_Init_FreeType(&ft))
			{
				throw std::runtime_error("Couldn't initialize Freetype\n");
			}

			if (std::atexit(Quit) != 0)
			{
				FT_Done_FreeType(ft);
				throw std::runtime_error("Failed to setup Freetype cleanup function\n");
			}

			initialized = true;
		}
	}

	void	Freetype::Quit()
	{
		if (initialized)
		{
			FT_Done_FreeType(ft);
			initialized = false;
		}
	}

	FT_Library	Freetype::getFT()
	{
		return ft;
	}

	FT_Library	ft = nullptr;

	bool Freetype::initialized = false;
}