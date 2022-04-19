#include "Object/Animation.class.hpp"
#include "Object/AssimpHelpers.hpp"
#include "Object/AssetManager.class.hpp"
#include "ColladaParser/ColladaSceneBuilder.class.hpp"
#include "CheckFileType.hpp"

#include <algorithm>

//	Fix for windows
# undef max
# undef min

namespace notrealengine
{

	Animation::Animation(const std::string& path, int index)
		: Asset({path}), duration(0), ticksPerSecond(1000), ticksFactor(1.0),
		type(Skeletal), bones(), currentFrame(0), nodes(), ended(false)
	{
		std::cout << "Loading animation " << index << " of '" << path;

		std::filesystem::path	fPath(path);
		if (!std::filesystem::exists(fPath))
		{
			std::cerr << "nre: Unable to open file \"" << path << "\"" << std::endl;
			return;
		}
		if (!IsReg(path))
		{
			std::cerr << "nre: Invalid file type" << std::endl;
			return ;
		}
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
		Setup(scene, index);
	}

	Animation::Animation(const std::string& name, std::map<std::string, Bone>& bones)
		: Asset(name), duration(0), ticksPerSecond(1000.0), ticksFactor(1.0),
		type(Solid), bones(bones), currentFrame(0), ended(false), nodes()
	{
		std::cout << "Creating animation from hard coded bones..." << std::endl;
		for (const auto& pair: bones)
		{
			const Bone& bone = pair.second;
			this->duration = std::max(this->duration, bone.getMaxTime());
		}
	}

	Animation::Animation(const Animation& ref)
	{
		*this = ref;
	}

	Animation& Animation::operator=(const Animation& ref)
	{
		this->duration = ref.duration;
		this->ticksPerSecond = ref.ticksPerSecond;
		this->ticksFactor = ref.ticksFactor;
		this->type = ref.type;
		this->bones = ref.bones;
		this->currentFrame = ref.currentFrame;
		this->ended = ref.ended;
		this->nodes = ref.nodes;
		return *this;
	}

	Animation::Animation(): duration(0.0), ticksPerSecond(0.0), ticksFactor(0.0),
		type(AnimType::Skeletal), bones(), currentFrame(0), ended(false), nodes()
	{
	}

	Animation::~Animation()
	{
	}

	void Animation::Setup(const void *scene, int index)
	{
		this->type = Skeletal;
		this->bones.clear();
		const void* rootNode;
#ifdef USING_EXTERNAL_LIBS
		const aiScene* aiscene = static_cast<const aiScene*>(scene);
		rootNode = aiscene->mRootNode;
		aiAnimation* animation = aiscene->mAnimations[index];
		const aiMetadata* metadata = aiscene->mMetaData;
		int upAxis = -1;
		aiscene->mMetaData->Get<int>("UpAxis", upAxis);
		this->name = animation->mName.C_Str();
#else
		const cpScene* cpscene = static_cast<const cpScene*>(scene);
		rootNode = cpscene->mRootNode;
		cpAnimation* animation = cpscene->mAnimations[index];
		this->name = animation->mName;
#endif
		this->duration = animation->mDuration;
		this->ticksPerSecond = animation->mTicksPerSecond;
		this->ticksFactor = this->ticksPerSecond / 1000;
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
		processNode(rootNode, animation, 0);
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

	const double	Animation::getTicksFactor(void) const
	{
		return this->ticksFactor;
	}

	const Animation::AnimType	Animation::getType(void) const
	{
		return this->type;
	}

	std::vector<std::shared_ptr<Animation>>	LoadAnimations(const std::string& path)
	{
		std::cout << "Loading animations of " << path;

		std::vector<std::shared_ptr<Animation>> animations;

		std::filesystem::path	fPath(path);
		if (!std::filesystem::exists(fPath))
		{
			std::cerr << "nre: Unable to open file \"" << path << "\"" << std::endl;
			return animations;
		}
		if (!IsReg(path))
		{
			std::cerr << "nre: Invalid file type" << std::endl;
			return animations;
		}
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
			return animations;
		}
		if (scene->HasAnimations() == false || scene->mNumAnimations == 0)
		{
			std::cerr << path << " does not contain any animation" << std::endl;
			return animations;
		}
		for(unsigned int i = 0; i < scene->mNumAnimations; i++)
		{
			std::shared_ptr<Animation> anim =
				std::shared_ptr<Animation>(new Animation());
			anim->Setup(scene, i);
			animations.push_back(anim);
			AssetManager::getInstance().addAsset(anim);
		}
		return animations;
	}

}
