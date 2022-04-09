#include "Object/ObjectImporter.class.hpp"

//	Windows fix
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
# undef max
# undef min
#endif

namespace notrealengine
{
	ObjectImporter::ObjectImporter(): max(), min(), isRangeInit(false),
		meshes(), directory(""), path(""), bones(), nbBones(0), axis(Axis::Y_UP)
	{
	}

	ObjectImporter::~ObjectImporter()
	{
	}
}
