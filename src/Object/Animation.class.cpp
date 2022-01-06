#include "Object/Animation.class.hpp"

//	Fix for assimp
#undef max
#undef min
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

namespace notrealengine
{
	Animation::Animation(const std::string& path, int index)
		: type(Skeletal)
	{
		for (int i = 0; i < MAX_BONES; i++)
		{
			mat[i] = mft::mat4();
		}
		Assimp::Importer	importer;
		const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate);
		if (!scene || !scene->mRootNode)
		{
			std::cerr << "assimp: " << importer.GetErrorString() << std::endl;
			return;
		}
		if (scene->HasAnimations() == false || index >= scene->mNumAnimations)
		{
			std::cerr << path << " does not have " << index << " animations" << std::endl;
			return;
		}
		aiAnimation* animation = scene->mAnimations[index];
		//std::cout << "Animation " << name << " uses ";
		//std::cout << animation->mNumChannels << " bones" << std::endl;
		for (unsigned int i = 0; i < animation->mNumChannels; i++)
		{
			aiNodeAnim* frame = animation->mChannels[i];
			//std::cout << "Bone " << frame->mNodeName.C_Str();
			//std::cout << " has " << frame->mNumPositionKeys << " positions, ";
			//std::cout << frame->mNumRotationKeys << " rotations and";
			//std::cout << frame->mNumScalingKeys << " scales" << std::endl;
			//mat[i] *= mft::mat4::scale(AssimpToMftVec3(frame->mPositionKeys[35].mValue));
			//mat[i] *= mft::mat4::rotate(frame->mRotationKeys[35].mValue
			//mat[i] *= mft::mat4::translate(AssimpToMftVec3(frame->mScalingKeys[35].mValue));
		}
	}

	Animation::~Animation()
	{
	}

	const mft::mat4* Animation::getMatrices() const
	{
		return mat;
	}

	mft::vec3	Animation::AssimpToMftVec3(aiVector3D& vec) const
	{
		return (mft::vec3(vec.x, vec.y, vec.z));
	}

	void	Animation::playAnimation(GLObject& object)
	{

	}
}