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
		bones(), currentFrame(0), nodes()
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
		}
		processNode(scene->mRootNode, animation, 0);
	}

	Animation::~Animation()
	{
	}

	void Animation::processNode(aiNode* node, aiAnimation* animation, int parentId)
	{
		MatrixNode newNode;
		unsigned int	currentId = nodes.size();
		newNode.name = node->mName.data;
		newNode.parentId = parentId;
		newNode.transform = AssimpToMftMatrix(node->mTransformation);
		nodes.push_back(newNode);
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
			processNode(node->mChildren[i], animation, currentId);
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
		std::map<std::string, BoneInfo>& objBones = object.getBones();
		for (unsigned int i = 0; i < this->nodes.size(); i++)
		{
			MatrixNode& node = this->nodes[i];

			//	If a bone of the animation is associated with this node,
			//	use its animation transform

			if (this->bones.contains(node.name))
			{
				node.transform = this->bones[node.name].getTransform(this->currentFrame)
					* this->nodes[node.parentId].transform;
			}
			//	Otherwise, use the original node's transform
			else
			{
				node.transform = node.transform
					* this->nodes[node.parentId].transform;
			}
			//	If a bone of the object is associated with this node,
			//	update its transform with what we just computed
			if (objBones.contains(node.name))
			{
				BoneInfo& bone = objBones[node.name];

				bone.modelMatrix = node.transform;
				bone.localMatrix = bone.offsetMatrix * bone.modelMatrix;
			}
		}
		object.bindBones();
	}
}
