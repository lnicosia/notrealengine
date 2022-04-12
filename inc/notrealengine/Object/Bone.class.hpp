#ifndef _BONE_CLASS_H_
# define _BONE_CLASS_H_

#include "mft/mft.hpp"
#include "ColladaParser/ColladaSceneBuilder.class.hpp"

//	Fix for windows 
# undef max
# undef min

#ifdef USING_EXTERNAL_LIBS
# include "assimp/Importer.hpp"
# include "assimp/scene.h"
# include "assimp/postprocess.h"
#endif

#include <string>
#include <vector>

namespace notrealengine
{
	struct	VecKeyFrame
	{
		mft::vec3	vec;
		double		time;
		VecKeyFrame(): vec(), time() {}
		VecKeyFrame(const mft::vec3& vec, const double& time): vec(vec), time(time) {}
	};

	struct	QuatKeyFrame
	{
		mft::quat	quat;
		double		time;
		QuatKeyFrame(): quat(), time() {}
		QuatKeyFrame(const mft::quat& quat, const double& time): quat(quat), time(time) {}
	};

	class Bone
	{
	public:
		Bone();
		Bone(const std::string& name, const int id, const void* node);
		Bone(const std::string& name,
			const std::vector<VecKeyFrame>& positions,
			const std::vector<QuatKeyFrame>& rotations,
			const std::vector<VecKeyFrame>& scales);
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
			getNbTransforms( void ) const;
		const unsigned int
			getNbPositions( void ) const;
		const unsigned int
			getNbRotations( void ) const;
		const unsigned int
			getNbScales( void ) const;
		const double
			getMaxTime( void ) const;

		//	Setters

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
		std::string name;

		std::vector<VecKeyFrame>	positions;
		std::vector<QuatKeyFrame>	rotations;
		std::vector<VecKeyFrame>	scales;

		unsigned int				nbPositions;
		unsigned int				nbRotations;
		unsigned int				nbScales;

		//	Precomputed matrices for each keyframe by combining pos, rot and scales
		//	-- not needed anymore but can be usefull for debugging
		std::vector<mft::mat4>		transforms;
	};

}

#endif // !_BONE_CLASS_H_
