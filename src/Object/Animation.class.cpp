#include "Object/Animation.class.hpp"
#include "Object/AssimpHelpers.hpp"
#include "ColladaParser/ColladaSceneBuilder.class.hpp"

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

#ifdef USING_EXTERNAL_LIBS
		Assimp::Importer	importer;
		const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate);
#else
		ColladaSceneBuilder importer;
		const cpScene* scene = importer.ReadFile(path, 0);
#endif
		if (!scene || !scene->mRootNode)
		{
#ifdef USING_EXTERNAL_LIBS
			std::cerr << "assimp: " << importer.GetErrorString() << std::endl;
#else
			std::cerr << "Custom parser failed to import animation" << std::endl;
#endif
			return;
		}
		if (scene->HasAnimations() == false || index >= scene->mNumAnimations)
		{
			std::cerr << path << " does not have " << index << " animations" << std::endl;
			return;
		}
#ifdef USING_EXTERNAL_LIBS
		aiAnimation* animation = scene->mAnimations[index];
#else
		cpAnimation* animation = scene->mAnimations[index];
#endif
		this->duration = animation->mDuration;
		this->ticksPerSecond = animation->mTicksPerSecond;
		for (unsigned int i = 0; i < animation->mNumChannels; i++)
		{
			if (animation->mChannels[i] != nullptr)
			{
#ifdef USING_EXTERNAL_LIBS
				aiNodeAnim*	bone = animation->mChannels[i];
				std::string name = bone->mNodeName.data;
#else
				cpNodeAnim*	bone = animation->mChannels[i];
				std::string name = bone->mNodeName;
#endif
				this->bones.emplace(name, Bone(name, 0, bone));
			}
		}
		processNode(scene->mRootNode, animation, 0);
#ifndef USING_EXTERNAL_LIBS
		delete scene;
#endif
	}

	Animation::~Animation()
	{
	}

	void Animation::processNode(const void* nnode, const void* aanimation, int parentId)
	{
		static int count = 0;
		count++;
		AnimNode newNode;
		unsigned int	currentId = this->nodes.size();
#ifdef USING_EXTERNAL_LIBS
		const aiAnimation* animation = static_cast<const aiAnimation*>(aanimation);
		const aiNode* node = static_cast<const aiNode*>(nnode);
		newNode.name = node->mName.data;
		newNode.transform = AssimpToMftMatrix(node->mTransformation);
#else
		const cpAnimation* animation = static_cast<const cpAnimation*>(aanimation);
		const cpNode* node = static_cast<const cpNode*>(nnode);
		newNode.name = node->mName;
		newNode.transform = node->mTransformation;
#endif
		
		newNode.parentId = parentId;
		
		this->nodes.push_back(newNode);
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
