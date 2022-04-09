#include "CheckFileType.hpp"

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
# define NRE_WINDOWS
#else
# include <unistd.h>
# include <sys/stat.h>
#endif

#include <iostream>

namespace notrealengine
{
	bool	IsReg(const std::string& path)
	{
#ifdef NRE_WINDOWS
		return true;
#else
		struct stat fileStats;
		lstat(path.c_str(), &fileStats);
		if (!S_ISREG(fileStats.st_mode))
		{
			return false;
		}
		return true;
#endif
	}

	bool	IsReg(const std::filesystem::path& path)
	{
#ifdef NRE_WINDOWS
		return true;
#else
		struct stat fileStats;
		lstat(path.string().c_str(), &fileStats);
		if (!S_ISREG(fileStats.st_mode))
		{
			return false;
		}
		return true;
#endif
	}
}