#include "Object/Animation.class.hpp"
#include "Object/AssimpHelpers.hpp"

//	Fix for assimp
#undef max
#undef min
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

namespace notrealengine
{
	Animation::Animation(const std::string& path, int index)
		: Asset({path}), type(Skeletal)
	{
		std::cout << "Loading animation " << index << " of " << path << std::endl;
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
		processNode(scene->mRootNode);
		for (unsigned int i = 0; i < animation->mNumChannels; i++)
		{
			aiNodeAnim* node = animation->mChannels[i];
			//std::cout << "Bone " << frame->mNodeName.C_Str();
			//std::cout << " has " << frame->mNumPositionKeys << " positions, ";
			//std::cout << frame->mNumRotationKeys << " rotations and";
			//std::cout << frame->mNumScalingKeys << " scales" << std::endl;
			//mat[i] *= mft::mat4::scale(AssimpToMftVec3(frame->mPositionKeys[35].mValue));
			//mat[i] *= mft::mat4::rotate(frame->mRotationKeys[35].mValue
			//mat[i] *= mft::mat4::translate(AssimpToMftVec3(frame->mScalingKeys[35].mValue));
			//Bone	bone(node->mNodeName.C_Str(), 0, node);
			//bones.emplace(node->mNodeName.C_Str(), Bone(node->mNodeName.C_Str(), 0, node));
			//std::cout << std::endl;
		}
	}

	Animation::~Animation()
	{
	}

	void Animation::processNode(aiNode* node)
	{
		std::cout << "Node '" << node->mName.C_Str() << "' has " << node->mNumChildren << " children" << std::endl;
		std::cout << "Transform matrix = " << AssimpToMftMatrix(node->mTransformation) << std::endl;
		for (int i = 0; i < node->mNumChildren; i++)
		{
			processNode(node->mChildren[i]);
		}
	}

	const mft::mat4* Animation::getMatrices() const
	{
		return mat;
	}

	const std::string Animation::getAssetType() const
	{
		return std::string("Animation");
	}

	void	Animation::playAnimation(GLObject& object)
	{
		std::map<std::string, BoneInfo>& bones = object.getBones();
		for(auto& pair: bones)
		{
			mft::mat4	transform;
			std::string name = pair.first;
			if (!bones.contains(name))
				continue;
			std::cout << "Bone '" << pair.second.name << "':" << std::endl;
			std::cout << "Original local matrix = " << pair.second.localMatrix << std::endl;
			pair.second.localMatrix =
			mft::mat4::translate(this->bones[name].getPosition(0))
			* mft::mat4::rotate(this->bones[name].getRotation(0))
			* mft::mat4::scale(this->bones[name].getScale(0));
			std::cout << "New local matrix = " << pair.second.localMatrix << std::endl;
			std::cout << "Original global matrix = " << pair.second.globalMatrix << std::endl;
			pair.second.globalMatrix = mft::mat4::inverse(pair.second.localMatrix);
			std::cout << "New global matrix = " << pair.second.globalMatrix << std::endl;
		}
	}
}
