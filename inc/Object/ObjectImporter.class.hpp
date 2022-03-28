#ifndef _OBJECT_IMPORTER_CLASS_H_
#define _OBJECT_IMPORTER_CLASS_H_

#include "Object/Mesh.class.hpp"

#include <string>
#include <vector>
#include <map>

namespace notrealengine
{
	struct BoneInfo
	{
		int	id;
		std::string	name;
		mft::mat4	offsetMatrix;
		mft::mat4	localMatrix;
		mft::mat4	modelMatrix;
		mft::mat4	originalMatrix;
	};

	class ObjectImporter
	{
	public:
		ObjectImporter();
		virtual ~ObjectImporter();

		mft::vec3	max;
		mft::vec3	min;
		bool		isRangeInit;

		std::vector<std::shared_ptr<Mesh>>	meshes;

		std::string	directory;
		std::string	path;

		std::map<std::string, BoneInfo>	bones;
		int	nbBones;

		virtual void
			ReadFile(const std::string& path, unsigned int flags) = 0;

	private:

	};
}
#endif // !_OBJECT_IMPORTER_CLASS_H_
