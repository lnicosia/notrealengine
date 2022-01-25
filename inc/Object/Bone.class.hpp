#ifndef _BONE_CLASS_H_
# define _BONE_CLASS_H_

#include "mft/mft.hpp"

//	Fix for assimp
#undef max
#undef min
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

#include <string>
#include <vector>

namespace notrealengine
{
	struct	VecKeyFrame
	{
		mft::vec3	vec;
		float			time;
	};

	struct	QuatKeyFrame
	{
		mft::quat	quat;
		float			time;
	};

	class Bone
	{
	public:
		Bone();
		Bone(const std::string& name, const int id, const aiNodeAnim* node);
		Bone(const Bone& ref);
		Bone& operator=(const Bone& ref);
		~Bone();

		//	Accessors

		const mft::vec3&
			getPosition(const int index) const;
		const mft::quat&
			getRotation(const int index) const;
		const mft::vec3&
			getScale(const int index) const;
		const std::string&
			getName( void ) const;

		//	Setters

		void
			setGlobalMatrix(const mft::mat4& ref);
		void
			setLocalMatrix(const mft::mat4& ref);
		void
			setPosition(const int index, const mft::mat4& pos);
		void
			setRotation(const int index, const mft::quat& rotation);
		void
			setScale(const int index, const mft::mat4& scale);

	private:
		int			id;
		std::string name;
		mft::mat4	globalMatrix;
		mft::mat4	localMatrix;

		std::vector<VecKeyFrame>	positions;
		std::vector<QuatKeyFrame>	rotations;
		std::vector<VecKeyFrame>	scales;
	};

}

#endif