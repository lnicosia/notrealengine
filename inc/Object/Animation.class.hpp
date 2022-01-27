#ifndef _ANIMATION_CLASS_H_
# define _ANIMATION_CLASS_H_

#include "mft/mft.hpp"
#include "Object/GLObject.class.hpp"
#include "Object/Asset.class.hpp"
#include "Object/Bone.class.hpp"

//	Fix for assimp
#undef max
#undef min
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"


#define MAX_BONES 100

namespace notrealengine
{
	struct	MatrixNode
	{
		std::string							name;
		mft::mat4								transform;
		std::vector<MatrixNode>	children;
	};

	class Animation: public Asset
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
			getMatrices( void ) const;
		const std::map<std::string, Bone>&
			getBones( void ) const;

		void
			processNode(aiNode* node, aiAnimation* animation, MatrixNode& matrixNode);
		void
			playAnimation(GLObject& object, int frame);
		void
			computeBoneTransform(MatrixNode &node, const mft::mat4& parentMat);

		virtual const std::string
			getAssetType() const;

	private:
		double		duration;
		double		ticksPerSecond;
		int				type;
		mft::mat4	mat[MAX_BONES];
		std::map<std::string, Bone>	bones;

		int	currentFrame;

		MatrixNode	matrixStack;
	};
}
#endif
