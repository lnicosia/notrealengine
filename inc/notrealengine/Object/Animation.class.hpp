#ifndef _ANIMATION_CLASS_H_
# define _ANIMATION_CLASS_H_

#include "mft/mft.hpp"
#include "Object/Asset.class.hpp"
#include "Object/Bone.class.hpp"
#include "UsingExternalLibs.hpp"

#ifdef USING_EXTERNAL_LIBS
//	Fix for assimp
# undef max
# undef min
# include "assimp/Importer.hpp"
# include "assimp/scene.h"
# include "assimp/postprocess.h"
#endif

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
	public:
		enum AnimType
		{
			Skeletal,
			Solid
		};
		Animation(const std::string& path, int index);
		Animation(const std::string& name, std::map<std::string, Bone>& bones);
		~Animation();

		const std::map<std::string, Bone>&
			getBones( void ) const;
		std::vector<AnimNode>&
			getNodes( void );
		const double
			getDuration( void ) const;
		const double
			getTicksPerSecond( void ) const;
		const double
			getTicksFactor( void ) const;
		const AnimType
			getType( void ) const;

		//	Asset class

		const std::string
			getAssetType() const override;

	private:
		double		duration;
		double		ticksPerSecond;
		//	TicksPerSecond / 1000
		double		ticksFactor;
		AnimType	type;
		std::map<std::string, Bone>	bones;
		//std::map<std::string, std::shared_ptr<Mesh>> meshes;

		int	currentFrame;

		bool	ended;

		std::vector<AnimNode>	nodes;

		/**	Run through the importer's node hierarchy to save it with its
		**	transformations
		*/
		void
			processNode(const void* nnode, const void* aanimation, int parentId);
	};
}
#endif // !_ANIMATION_CLASS_H_
