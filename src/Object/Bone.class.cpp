#include "Object/Bone.class.hpp"
#include "Object/AssimpHelpers.hpp"

namespace notrealengine
{
	Bone::Bone(): id(0), name(), globalMatrix(), localMatrix(),
	positions(), rotations(), scales(), transforms(), modelMatrices(),
	nbPositions(0), nbRotations(0), nbScales(0)
	{

	}

	Bone::Bone(const std::string& name, const int id, const aiNodeAnim* node)
	: id(0), name(name), globalMatrix(), localMatrix(),
	positions(), rotations(), scales(), transforms(), modelMatrices(),
	nbPositions(0), nbRotations(0), nbScales(0)
	{
		//std::cout << "Bone " << name << std::endl;
		for (unsigned int j = 0; j < node->mNumPositionKeys; j++)
		{
			VecKeyFrame	keyFrame;
			keyFrame.vec = AssimpToMftVec3(node->mPositionKeys[j].mValue);
			keyFrame.time = node->mPositionKeys[j].mTime;
			//std::cout << "Time = " << keyFrame.time << std::endl;
			positions.push_back(keyFrame);
			nbPositions++;
		}
		//std::cout << std::endl;
		for (unsigned int j = 0; j < node->mNumRotationKeys; j++)
		{
			QuatKeyFrame	keyFrame;
			keyFrame.quat = AssimpToMftQuat(node->mRotationKeys[j].mValue);
			keyFrame.time = node->mRotationKeys[j].mTime;
			//std::cout << "Time = " << keyFrame.time << std::endl;
			rotations.push_back(keyFrame);
			//std::cout << "Rotation " << j << " = " << keyFrame.quat << std::endl;
			nbRotations++;
		}
		//std::cout << std::endl;
		for (unsigned int j = 0; j < node->mNumScalingKeys; j++)
		{
			VecKeyFrame	keyFrame;
			keyFrame.vec = AssimpToMftVec3(node->mScalingKeys[j].mValue);
			keyFrame.time = node->mScalingKeys[j].mTime;
			//std::cout << "Time = " << keyFrame.time << std::endl;
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
			/*if (j == 70)
			{
				std::cout << "Bone " << this->name << " transform " << j << ":" << std::endl;
				std::cout << "Scale = " << scales[j].vec << std::endl;
				std::cout << "Rotation = " << rotations[j].quat << std::endl;
				std::cout << "Position = " << positions[j].vec << std::endl;
				std::cout << "Matrix = " << transform << std::endl;
			}*/
			transforms.push_back(transform);
		}
	}

	Bone::Bone(const Bone& ref):
		id(ref.id), name(ref.name),
		globalMatrix(ref.globalMatrix), localMatrix(ref.localMatrix),
		positions(ref.positions), rotations(ref.rotations), scales(ref.scales),
		transforms(ref.transforms), modelMatrices(ref.modelMatrices),
		nbPositions(ref.nbPositions), nbRotations(ref.nbRotations), nbScales(ref.nbScales)
	{

	}

	Bone&	Bone::operator=(const Bone& ref)
	{
		this->id = ref.id;
		this->name = ref.name;
		this->globalMatrix = ref.globalMatrix;
		this->localMatrix = ref.localMatrix;
		this->positions = ref.positions;
		this->rotations = ref.rotations;
		this->scales = ref.scales;
		this->transforms = ref.transforms;
		this->modelMatrices = ref.modelMatrices;
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
			//throw std::out_of_range ("Index " + std::to_string(keyFrame) + " is out of bone '"
			//+ name + "' transforms range");
		return transforms[keyFrame];
	}

	const mft::mat4	Bone::getTransform(const unsigned int frameTime) const
	{
		return (
			mft::mat4::scale(getScale(frameTime))
			* mft::mat4::rotate(getRotation(frameTime))
			* mft::mat4::translate(getPosition(frameTime))
			);
	}

	const  mft::vec3	Bone::getPosition(const unsigned int frameTime) const
	{
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

	//	Setters

	void Bone::setGlobalMatrix(const mft::mat4& ref)
	{
		this->globalMatrix = ref;
	}

	void Bone::setLocalMatrix(const mft::mat4& ref)
	{
		this->localMatrix = ref;
	}

	void Bone::updateTransforms(const mft::mat4& mat)
	{
		for (unsigned int j = 0; j < transforms.size(); j++)
		{
			transforms[j] *= mat;
		}
	}

}
