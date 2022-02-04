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

		const mft::mat4
			getKeyFrameTransform(const unsigned int keyFrame) const;
		const mft::mat4
			getTransform(const unsigned int frameTime) const;
		const mft::vec3
			getPosition(const unsigned int frameTime) const;
		const mft::quat
			getRotation(const unsigned int frameTime) const;
		const mft::vec3
			getScale(const unsigned int frameTime) const;
		const std::string&
			getName( void ) const;
		const unsigned int
			getNbTransforms() const;
		const unsigned int
			getNbPositions() const;
		const unsigned int
			getNbRotations() const;
		const unsigned int
			getNbScales() const;

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

		void
			updateTransforms(const mft::mat4& mat);

			mft::mat4	modelMatrix;

	private:
		int			id;
		std::string name;
		mft::mat4	globalMatrix;
		mft::mat4	localMatrix;


		std::vector<VecKeyFrame>	positions;
		std::vector<QuatKeyFrame>	rotations;
		std::vector<VecKeyFrame>	scales;

		unsigned int				nbPositions;
		unsigned int				nbRotations;
		unsigned int				nbScales;

		std::vector<mft::mat4>		transforms;
		std::vector<mft::mat4>		modelMatrices;
	};

}

#endif
