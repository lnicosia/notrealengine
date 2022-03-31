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
		std::cout << "Loading animation " << index << " of '" << path;

#ifdef USING_EXTERNAL_LIBS
		std::cout << "' with assimp..." << std::endl;
		Assimp::Importer	importer;
		const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate);
#else
	std::cout << "' with custom parser..." << std::endl;
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
			std::cerr << "Animation " << index << " does not exist" << std::endl;
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

	Animation::Animation(const std::string& name, std::map<std::string, Bone>& bones)
		: Asset(name), duration(0), ticksPerSecond(1000.0), type(Solid), bones(bones),
		currentFrame(0), ended(false), nodes()
	{
		std::cout << "Loading animation from bones..." << std::endl;
		for (const auto& pair: bones)
		{
			const Bone& bone = pair.second;
			this->duration = std::max(this->duration, bone.getMaxTime());
		}
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
		return this->duration;
	}

	const double	Animation::getTicksPerSecond(void) const
	{
		return this->ticksPerSecond;
	}

	const Animation::AnimType	Animation::getType(void) const
	{
		return this->type;
	}

}
