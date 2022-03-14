#include "Object/ObjectImporter.class.hpp"

namespace notrealengine
{
	ObjectImporter::ObjectImporter(): max(), min(), isRangeInit(false),
		meshes(), directory(""), path(""), bones(), nbBones(0)
	{
	}

	ObjectImporter::~ObjectImporter()
	{
	}
}
