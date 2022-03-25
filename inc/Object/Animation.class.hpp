#ifndef _ANIMATION_CLASS_H_
# define _ANIMATION_CLASS_H_

#include "mft/mft.hpp"
#include "Object/Asset.class.hpp"
#include "Object/Bone.class.hpp"

//	Fix for assimp
#undef max
#undef min
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

#include <map>

#define MAX_BONES 100

namespace notrealengine
{
	struct	AnimNode
	{
		std::string				name;
		mft::mat4				transform;
		unsigned int			parentId;
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

		const std::map<std::string, Bone>&
			getBones( void ) const;
		std::vector<AnimNode>&
			getNodes( void );
		const double
			getDuration( void ) const;

		void
			processNode(cpNode* node, cpAnimation* animation, int parentId);

		virtual const std::string
			getAssetType() const;

	private:
		double		duration;
		double		ticksPerSecond;
		int				type;
		std::map<std::string, Bone>	bones;

		int	currentFrame;

		bool	ended;

		std::vector<AnimNode>	nodes;
	};
}
#endif
