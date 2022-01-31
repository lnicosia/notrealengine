#include "Object/Animation.class.hpp"
#include "Object/AssimpHelpers.hpp"

//	Fix for assimp
#undef max
#undef min
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

#include <algorithm>

namespace notrealengine
{
	Animation::Animation(const std::string& path, int index)
		: Asset({path}), duration(0), ticksPerSecond(0), type(Skeletal),
		bones(), matrixStack(), currentFrame(0)
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
		this->duration = animation->mDuration;
		this->ticksPerSecond = animation->mTicksPerSecond;
		//std::cout << "Animation " << name << " uses ";
		//std::cout << animation->mNumChannels << " bones" << std::endl;
		std::vector<std::string> bonesName;
		for (unsigned int i = 0; i < animation->mNumChannels; i++)
		{
			if (animation->mChannels[i] != nullptr)
			{
				aiNodeAnim*	bone = animation->mChannels[i];
				std::string name = bone->mNodeName.data;
				bonesName.push_back(name);
				bones.emplace(name, Bone(name, 0, bone));
			}
			//std::cout << "Animation uses bone '" << bonesName[i] << "'" << std::endl;
		}
		processNode(scene->mRootNode, animation, this->matrixStack);
	}

	Animation::~Animation()
	{
	}

	void Animation::processNode(aiNode* node, aiAnimation* animation, MatrixNode& matrixNode)
	{
		matrixNode.transform = AssimpToMftMatrix(node->mTransformation);
		matrixNode.name = node->mName.data;
		std::map<std::string, Bone>::iterator it;
		it = bones.find(node->mName.data);
		if (it == bones.end())
		{
			//std::cout << "Node '" << node->mName.data << "' relates to a bone" << std::endl;
			/*for(auto& pair: bones)
			{
				pair.second.updateTransforms(transform);
			}*/
		}
		for (int i = 0; i < node->mNumChildren; i++)
		{
			MatrixNode newNode;
			processNode(node->mChildren[i], animation, newNode);
			matrixNode.children.push_back(newNode);
		}
	}

	const mft::mat4* Animation::getMatrices( void ) const
	{
		return mat;
	}

	const std::map<std::string, Bone>& Animation::getBones( void ) const
	{
		return bones;
	}

	const std::string Animation::getAssetType() const
	{
		return std::string("Animation");
	}

	void	Animation::playAnimation(GLObject& object, int frame)
	{
		this->currentFrame = frame;
		computeBoneTransform(matrixStack, mft::mat4());
		std::map<std::string, BoneInfo>& bones = object.getBones();
		for(auto& pair: bones)
		{
			mft::mat4	transform;
			std::string name = pair.first;
			if (!this->bones.contains(name))
				continue;
			//std::cout << "Bone '" << pair.second.name << "':" << std::endl;
			//std::cout << "Nb position = " << this->bones[names].getPositions().size() << std::endl;
			//std::cout << "Original local matrix = " << pair.second.localMatrix << std::endl;
			//pair.second.modelMatrix = this->bones[name].getTransform(frame) * pair.second.fromParentMatrix;
			pair.second.modelMatrix = this->bones[name].modelMatrix;
			/*pair.second.modelMatrix = mft::mat4::translate(this->bones[name].getPosition(frame).vec)
			* mft::mat4::rotate(this->bones[name].getRotation(frame).quat)
			* mft::mat4::scale(this->bones[name].getScale(frame).vec) * pair.second.fromParentMatrix ;
			if (!(this->bones[name].getPosition(frame).time == this->bones[name].getRotation(frame).time
			&& this->bones[name].getPosition(frame).time == this->bones[name].getScale(frame).time))
				std::cout << "Different time stamps for same keyframe!" << std::endl;*/
			//std::cout << "New local matrix = " << pair.second.localMatrix << std::endl;
			//std::cout << "Original global matrix = " << pair.second.modelMatrix << std::endl;
			pair.second.localMatrix = pair.second.offsetMatrix * pair.second.modelMatrix;
			//std::cout << "New global matrix = " << pair.second.modelMatrix << std::endl;
		}
		object.bindBones();
	}

	void Animation::computeBoneTransform(MatrixNode &node, const mft::mat4& parentMat)
	{
		mft::mat4 transform;
		//	If there is a bone with this name, take its transform
		//	at the current frame
		if (bones.contains(node.name))
		{
			transform = this->bones[node.name].getTransform(this->currentFrame) * parentMat;
			//transform = node.transform * parentMat;
			this->bones[node.name].modelMatrix = transform;
			//std::cout << node.name << "'" << std::endl;
		}
		//	Otherwise, take the current matrix stack node's transform
		else
			transform = node.transform * parentMat;
		for (auto& child: node.children)
		{
			computeBoneTransform(child, transform);
		}
	}
}
