#include "Object/Bone.class.hpp"
#include "Object/AssimpHelpers.hpp"

namespace notrealengine
{
	Bone::Bone(): name(),
	positions(), rotations(), scales(), transforms(),
	nbPositions(0), nbRotations(0), nbScales(0)
	{

	}

	Bone::Bone(const std::string& name, const int id, const void* nnode)
	: name(name),
	positions(), rotations(), scales(), transforms(),
	nbPositions(0), nbRotations(0), nbScales(0)
	{

#ifdef USING_EXTERNAL_LIBS
		const aiNodeAnim* node = static_cast<const aiNodeAnim*>(nnode);
#else
		const cpNodeAnim* node = static_cast<const cpNodeAnim*>(nnode);
#endif // USING_EXTERNAL_LIBS
		for (unsigned int j = 0; j < node->mNumPositionKeys; j++)
		{
			VecKeyFrame	keyFrame;
#ifdef USING_EXTERNAL_LIBS
			keyFrame.vec = AssimpToMftVec3(node->mPositionKeys[j].mValue);
#else
			keyFrame.vec = node->mPositionKeys[j].mValue;
#endif
			keyFrame.time = node->mPositionKeys[j].mTime;
			positions.push_back(keyFrame);
			nbPositions++;
		}
		for (unsigned int j = 0; j < node->mNumRotationKeys; j++)
		{
			QuatKeyFrame	keyFrame;
#ifdef USING_EXTERNAL_LIBS
			keyFrame.quat = AssimpToMftQuat(node->mRotationKeys[j].mValue);
#else
			keyFrame.quat = node->mRotationKeys[j].mValue;
#endif
			keyFrame.time = node->mRotationKeys[j].mTime;
			rotations.push_back(keyFrame);
			nbRotations++;
		}
		for (unsigned int j = 0; j < node->mNumScalingKeys; j++)
		{
			VecKeyFrame	keyFrame;
#ifdef USING_EXTERNAL_LIBS
			keyFrame.vec = AssimpToMftVec3(node->mScalingKeys[j].mValue);
#else
			keyFrame.vec = node->mScalingKeys[j].mValue;
#endif
			keyFrame.time = node->mScalingKeys[j].mTime;
			scales.push_back(keyFrame);
			nbScales++;
		}

		int max = std::max(std::max(positions.size(), rotations.size()), scales.size());
		for (unsigned int j = 0; j < max; j++)
		{
			mft::mat4 translation, rotation, scale;
			if (j < positions.size())
				translation = mft::mat4::translate(positions[j].vec);
			else
				translation = mft::mat4::translate(positions[positions.size() - 1].vec);
			if (j < scales.size())
				scale = mft::mat4::scale(scales[j].vec);
			else
				scale = mft::mat4::scale(scales[scales.size() - 1].vec);
			if (j < rotations.size())
				rotation = mft::mat4::rotate(mft::quat::normalized(rotations[j].quat));
			else
				rotation = mft::mat4::rotate(mft::quat::normalized(rotations[rotations.size() - 1].quat));
			mft::mat4 transform = scale * rotation * translation;
			transforms.push_back(transform);
		}
	}

	Bone::Bone(const std::string& name,
		const std::vector<VecKeyFrame>& positions,
		const std::vector<QuatKeyFrame>& rotations,
		const std::vector<VecKeyFrame>& scales)
		: name(name), positions(positions), rotations(rotations), scales(scales),
		transforms(), nbPositions(0), nbRotations(0), nbScales(0)
	{
		this->nbPositions = static_cast<unsigned int>(positions.size());
		this->nbRotations = static_cast<unsigned int>(rotations.size());
		this->nbScales = static_cast<unsigned int>(scales.size());
	}

	Bone::Bone(const Bone& ref):
		name(ref.name),
		positions(ref.positions), rotations(ref.rotations), scales(ref.scales),
		transforms(ref.transforms),
		nbPositions(ref.nbPositions), nbRotations(ref.nbRotations), nbScales(ref.nbScales)
	{

	}

	Bone&	Bone::operator=(const Bone& ref)
	{
		this->name = ref.name;
		this->positions = ref.positions;
		this->rotations = ref.rotations;
		this->scales = ref.scales;
		this->transforms = ref.transforms;
		this->nbPositions = ref.nbPositions;
		this->nbRotations = ref.nbRotations;
		this->nbScales = ref.nbScales;
		return *this;
	}

	Bone::~Bone()
	{
	}

	//	Accessors

	const mft::mat4	Bone::getKeyFrameTransform(const unsigned int keyFrame) const
	{
		if (keyFrame >= transforms.size())
			return transforms[transforms.size() - 1];
		return transforms[keyFrame];
	}

	const mft::mat4	Bone::getTransform(const unsigned int frameTime) const
	{
		return (
			mft::mat4::translate(getPosition(frameTime))
			* mft::mat4::rotate(getRotation(frameTime))
			* mft::mat4::scale(getScale(frameTime))
			);
	}

	const  mft::vec3	Bone::getPosition(const unsigned int frameTime) const
	{
		if (this->positions.empty())
			return mft::vec3();
		for (unsigned int i = 0; i < this->nbPositions - 1; i++)
		{
			const VecKeyFrame& nextKeyFrame = this->positions[i + 1];
			if (frameTime < nextKeyFrame.time)
			{
				const VecKeyFrame& keyFrame = this->positions[i];
				float percentage = (frameTime - keyFrame.time) / (nextKeyFrame.time - keyFrame.time);
				return (mft::vec3(
					keyFrame.vec.x + (nextKeyFrame.vec.x - keyFrame.vec.x) * percentage,
					keyFrame.vec.y + (nextKeyFrame.vec.y - keyFrame.vec.y) * percentage,
					keyFrame.vec.z + (nextKeyFrame.vec.z - keyFrame.vec.z) * percentage
				));
			}
		}
		return this->positions[this->nbPositions - 1].vec;
	}

	const mft::quat	Bone::getRotation(const unsigned int frameTime) const
	{
		if (this->rotations.empty())
			return mft::quat();
		for (unsigned int i = 0; i < this->nbRotations - 1; i++)
		{
			const QuatKeyFrame& nextKeyFrame = this->rotations[i + 1];
			if (frameTime < nextKeyFrame.time)
			{
				const QuatKeyFrame& keyFrame = this->rotations[i];
				float percentage = (frameTime - keyFrame.time) / (nextKeyFrame.time - keyFrame.time);
				return (mft::quat::normalized(mft::quat::slerp(keyFrame.quat, nextKeyFrame.quat, percentage)));
			}
		}
		return this->rotations[this->nbRotations - 1].quat;
	}

	const  mft::vec3	Bone::getScale(const int unsigned frameTime) const
	{
		if (this->scales.empty())
			return mft::vec3(1.0f, 1.0f, 1.0f);
		for (unsigned int i = 0; i < this->nbScales - 1; i++)
		{
			const VecKeyFrame& nextKeyFrame = this->scales[i + 1];
			if (frameTime < nextKeyFrame.time)
			{
				const VecKeyFrame& keyFrame = this->scales[i];
				float percentage = (frameTime - keyFrame.time) / (nextKeyFrame.time - keyFrame.time);
				return (mft::vec3(
					keyFrame.vec.x + (nextKeyFrame.vec.x - keyFrame.vec.x) * percentage,
					keyFrame.vec.y + (nextKeyFrame.vec.y - keyFrame.vec.y) * percentage,
					keyFrame.vec.z + (nextKeyFrame.vec.z - keyFrame.vec.z) * percentage
				));
			}
		}
		return this->scales[this->nbScales - 1].vec;
	}

	const std::string&	Bone::getName( void ) const
	{
		return name;
	}

	const unsigned int	Bone::getNbTransforms( void ) const
	{
		return this->transforms.size();
	}

	const unsigned int	Bone::getNbPositions( void ) const
	{
		return this->nbPositions;
	}

	const unsigned int	Bone::getNbRotations( void ) const
	{
		return this->nbRotations;
	}

	const unsigned int	Bone::getNbScales( void ) const
	{
		return this->nbScales;
	}

	const double	Bone::getMaxTime( void ) const
	{
		double duration = 0;
		for (const auto& position: this->positions)
			duration = std::max(duration, position.time);
		for (const auto& rotation: this->rotations)
			duration = std::max(duration, rotation.time);
		for (const auto& scale: this->scales)
			duration = std::max(duration, scale.time);
		return duration;
	}

	//	Setters

	void Bone::updateTransforms(const mft::mat4& mat)
	{
		for (unsigned int j = 0; j < transforms.size(); j++)
		{
			transforms[j] *= mat;
		}
	}

}
