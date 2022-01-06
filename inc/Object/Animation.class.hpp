#ifndef _ANIMATION_CLASS_H_
# define _ANIMATION_CLASS_H_

#include "mft/mft.hpp"
#include "Object/GLObject.class.hpp"

//	Fix for assimp
#undef max
#undef min
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"


#define MAX_BONES 100

namespace notrealengine
{
	class Animation
	{
		enum Type
		{
			Skeletal,
			Raw
		};

	public:
		Animation(const std::string& path, int index);
		~Animation();

		const mft::mat4*
			getMatrices() const;

		void
			playAnimation(GLObject& object);
		mft::vec3
			AssimpToMftVec3(aiVector3D& vec) const;

	private:
		std::string	name;
		double		duration;
		double		ticksPerSecond;
		int			type;
		mft::mat4	mat[MAX_BONES];
		//std::vector<KeyFrame>	keyFrames;
	};
}
#endif