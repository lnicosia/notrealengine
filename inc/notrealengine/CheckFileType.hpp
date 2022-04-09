#ifndef _CHECK_FILE_TYPE_H_
#define _CHECK_FILE_TYPE_H_

#include <string>
#include <filesystem>

namespace notrealengine
{
	bool
		IsReg(const std::string& path);

	bool
		IsReg(const std::filesystem::path& path);
}

#endif