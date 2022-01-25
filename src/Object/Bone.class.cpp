#include "Object/Bone.class.hpp"
#include "Object/AssimpHelpers.hpp"

namespace notrealengine
{
	Bone::Bone(): id(0), name(), globalMatrix(), localMatrix(),
	positions(), rotations(), scales()
	{

	}

	Bone::Bone(const std::string& name, const int id, const aiNodeAnim* node)
	: id(0), name(), globalMatrix(), localMatrix(),
	positions(), rotations(), scales()
	{
		for (unsigned int j = 0; j < node->mNumPositionKeys; j++)
		{
			VecKeyFrame	keyFrame;
			keyFrame.vec = AssimpToMftVec3(node->mPositionKeys[j].mValue);
			keyFrame.time = node->mPositionKeys[j].mTime;
			//std::cout << "Time = " << keyFrame.time << std::endl;
			positions.push_back(keyFrame);
		}
		//std::cout << std::endl;
		for (unsigned int j = 0; j < node->mNumPositionKeys; j++)
		{
			QuatKeyFrame	keyFrame;
			keyFrame.quat = AssimpToMftQuat(node->mRotationKeys[j].mValue);
			keyFrame.time = node->mRotationKeys[j].mTime;
			//std::cout << "Time = " << keyFrame.time << std::endl;
			rotations.push_back(keyFrame);
		}
		//std::cout << std::endl;
		for (unsigned int j = 0; j < node->mNumScalingKeys; j++)
		{
			VecKeyFrame	keyFrame;
			keyFrame.vec = AssimpToMftVec3(node->mScalingKeys[j].mValue);
			keyFrame.time = node->mScalingKeys[j].mTime;
			//std::cout << "Time = " << keyFrame.time << std::endl;
			scales.push_back(keyFrame);
		}
	}

	Bone::Bone(const Bone& ref):
		id(ref.id), name(ref.name),
		globalMatrix(ref.globalMatrix), localMatrix(ref.localMatrix),
		positions(ref.positions), rotations(ref.rotations), scales(ref.scales)
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
		return *this;
	}

	Bone::~Bone()
	{
	}

	//	Accessors

	const mft::vec3&	Bone::getPosition(const int index) const
	{
		if (index >= positions.size())
			throw std::out_of_range ("Index" + std::to_string(index) + " is out of the bone's range");
		return positions[index].vec;
	}

	const mft::quat&	Bone::getRotation(const int index) const
	{
		if (index >= rotations.size())
			throw std::out_of_range ("Index" + std::to_string(index) + " is out of the bone's range");
		return rotations[index].quat;
	}

	const mft::vec3&	Bone::getScale(const int index) const
	{
		if (index >= scales.size())
			throw std::out_of_range ("Index" + std::to_string(index) + " is out of the bone's range");
		return scales[index].vec;
	}

	const std::string&	Bone::getName( void ) const
	{
		return name;
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

}
