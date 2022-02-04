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
		bones(), currentFrame(0), nodes(), ended(false)
	{
		std::cout << "Loading animation " << index << " of " << path << std::endl;
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
		for (unsigned int i = 0; i < animation->mNumChannels; i++)
		{
			if (animation->mChannels[i] != nullptr)
			{
				aiNodeAnim*	bone = animation->mChannels[i];
				std::string name = bone->mNodeName.data;
				this->bones.emplace(name, Bone(name, 0, bone));
			}
		}
		processNode(scene->mRootNode, animation, 0);
		for (auto& node: this->nodes)
		{
			//std::cout << "Node " << node.name << " transform = " << node.transform << std::endl;
		}
	}

	Animation::~Animation()
	{
	}

	void Animation::processNode(aiNode* node, aiAnimation* animation, int parentId)
	{
		static int count = 0;
		count++;
		AnimNode newNode;
		unsigned int	currentId = this->nodes.size();
		newNode.name = node->mName.data;
		newNode.parentId = parentId;
		newNode.transform = AssimpToMftMatrix(node->mTransformation);
		this->nodes.push_back(newNode);
		std::cout << "Reading node ";
		for (int i = 0; i < count; i++)
			std::cout << "  ";
		std::cout << newNode.name;
		if (this->bones.contains(newNode.name))
		{
			std::cout << " (used, has ";
			std::cout << this->bones[newNode.name].getNbPositions() << " position frames, ";
			std::cout << this->bones[newNode.name].getNbRotations() << " rotation frames and ";
			std::cout << this->bones[newNode.name].getNbScales() << " scale frames)";
		// << " ( " << animation->mNumChannels << " frames" << std::endl;
		}
		std::cout << std::endl;
		for (int i = 0; i < node->mNumChildren; i++)
		{
			processNode(node->mChildren[i], animation, currentId);
		}
		count--;
	}

	const std::map<std::string, Bone>& Animation::getBones( void ) const
	{
		return this->bones;
	}

	std::vector<AnimNode>& Animation::getNodes( void )
	{
		return this->nodes;
	}

	const std::string Animation::getAssetType( void ) const
	{
		return std::string("Animation");
	}

	const double	Animation::getDuration(void) const
	{
		return duration;
	}

}
