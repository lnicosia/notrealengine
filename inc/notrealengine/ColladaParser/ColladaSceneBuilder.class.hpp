#ifndef _COLLADA_SCENE_BUILDER_CLASS_H_
#define _COLLADA_SCENE_BUILDER_CLASS_H_

#include "ColladaParser/ColladaParser.class.hpp"

namespace notrealengine
{
	enum cpTextureType
	{
		diffuse,
		specular,
		ambient
	};
	struct cpTexture
	{
		std::string		mName;
		cpTextureType	type;
		std::string		path;
	};
	struct cpMaterial
	{
		std::string mName;
		unsigned int mNumDiffuses;
		unsigned int mNumSpeculars;
		unsigned int mNumAmbients;
		std::vector<cpTexture> mDiffuse;
		std::vector<cpTexture> mSpecular;
		std::vector<cpTexture> mAmbient;

		unsigned int
			GetTextureCount(cpTextureType type)
		{
			switch (type)
			{
			case cpTextureType::diffuse:
				return this->mNumDiffuses;
				break;
			case cpTextureType::specular:
				return this->mNumSpeculars;
				break;
			case cpTextureType::ambient:
				return this->mNumAmbients;
				break;
			}
		}

		void
			GetTexture(cpTextureType type, unsigned int index, std::string& str)
		{
			std::vector<cpTexture>* textures = nullptr;
			if (type == cpTextureType::diffuse)
				textures = &this->mDiffuse;
			else if (type == cpTextureType::specular)
				textures = &this->mSpecular;
			else if (type == cpTextureType::ambient)
				textures = &this->mAmbient;
			str = (*textures)[index].path;
		}
	};
	struct cpQuatKey
	{
		double		mTime;
		mft::quat	mValue;
	};
	struct cpVectorKey
	{
		double		mTime;
		mft::vec3 mValue;
	};
	//	Channel created from AnimationChannels and Node used to
	//	create cpAnimations
	struct Channel
	{
		std::string	target;
		std::string	transformId; // Delete if only index needs to be saved
		long long int	transformIndex;
		const ColladaParser::AnimationChannel* srcChannel;
		const ColladaParser::ColladaAccessor* timesAcc; // Delete if only the array needs to be saved
		const std::vector<float>*							times;
		const ColladaParser::ColladaAccessor*	valuesAcc; // Delete if only the array needs to be saved
		const std::vector<float>*							values;
	};
	struct cpNodeAnim
	{
		std::string mNodeName;
		unsigned int mNumPositionKeys;
		unsigned int mNumRotationKeys;
		unsigned int mNumScalingKeys;
		cpVectorKey* mPositionKeys;
		cpQuatKey*   mRotationKeys;
		cpVectorKey* mScalingKeys;
	};
	struct cpAnimation
	{
		cpNodeAnim**	mChannels;
		unsigned int	mNumChannels;
		double				mTicksPerSecond;
		double				mDuration;
		std::string		mName;

		cpAnimation(): mChannels(nullptr), mNumChannels(0), mTicksPerSecond(0),
			mDuration(0), mName("")
		{}

	};
	struct cpVertexWeight
	{
		unsigned int mVertexId;
		float mWeight;
	};
	struct cpBone
	{
		std::string mName;

		cpVertexWeight* mWeights;
		std::string	mNodeName;
		unsigned int	mNumWeights;

		mft::mat4 mOffsetMatrix;

		cpBone(): mName(""), mWeights(nullptr), mNodeName(""), mNumWeights(0)
		{}
	};
	struct cpFace
	{
		unsigned int* mIndices;
		unsigned int  mNumIndices;

		cpFace(): mIndices(nullptr), mNumIndices(0)
		{}

	};
	struct cpMesh
	{
		mft::vec3* mVertices;
		mft::vec3* mNormals;
		mft::vec3* mTextureCoords[MAX_TEXTURE_CHANNELS];
		unsigned int mNumUVComponents[MAX_TEXTURE_CHANNELS];
		mft::vec4* mColors[MAX_TEXTURE_CHANNELS];

		cpBone** mBones;
		cpFace*	mFaces;

		std::string		mName;

		unsigned int	mNumVertices;
		unsigned int	mNumFaces;
		unsigned int	mMaterialIndex;
		unsigned int	mNumBones;

		cpMesh(): mVertices(nullptr), mNormals(nullptr), mBones(nullptr), mFaces(nullptr),
			mNumBones(0), mNumFaces(0), mMaterialIndex(0), mNumVertices(0)
		{
			for (size_t i = 0; i < MAX_TEXTURE_CHANNELS; i++)
			{
				this->mTextureCoords[i] = nullptr;
				this->mNumUVComponents[i] = 0;
				this->mColors[i] = nullptr;
			}
		}

	};
	struct cpNode
	{
		cpNode** mChildren;
		cpNode* mParent;
		unsigned int* mMeshes;

		mft::mat4		mTransformation;

		std::string		mName;

		unsigned int	mNumChildren;
		unsigned int	mNumMeshes;

		cpNode(): mChildren(nullptr), mParent(nullptr), mMeshes(nullptr),
			mTransformation(), mName(""), mNumChildren(0), mNumMeshes()
		{}
	};
	struct cpScene
	{
		cpAnimation** mAnimations;
		cpMesh** mMeshes;
		cpMaterial** mMaterials;
		cpTexture** mTextures;

		cpNode* mRootNode;

		unsigned int	mNumAnimations;
		unsigned int	mNumMeshes;
		unsigned int	mNumMaterials;
		unsigned int	mNumTextures;

		cpScene(): mAnimations(nullptr), mMeshes(nullptr), mMaterials(nullptr),
			mTextures(nullptr), mRootNode(nullptr),
			mNumAnimations(0), mNumMeshes(0), mNumMaterials(0), mNumTextures(0)
		{}

		~cpScene()
		{
			for (unsigned int i = 0; i < mNumAnimations; i++)
			{
				if (this->mAnimations[i] != nullptr)
					deleteAnim(this->mAnimations[i]);
			}
			if (this->mAnimations != nullptr)
				delete [] this->mAnimations;

			for (unsigned int i = 0; i < mNumMeshes; i++)
			{
				if (this->mMeshes[i] != nullptr)
					deleteMesh(this->mMeshes[i]);
			}
			if (this->mMeshes != nullptr)
				delete [] this->mMeshes;

			for (unsigned int i = 0; i < mNumMaterials; i++)
			{
				if (this->mMaterials[i] != nullptr)
					delete this->mMaterials[i];
			}
			if (this->mMaterials != nullptr)
				delete [] this->mMaterials;

			for (unsigned int i = 0; i < mNumTextures; i++)
			{
				if (this->mTextures[i] != nullptr)
					delete this->mTextures[i];
			}
			if (this->mTextures != nullptr)
				delete [] this->mTextures;
			if (this->mRootNode != nullptr)
				deleteNode(this->mRootNode);
		}

		bool HasAnimations() const { return this->mNumAnimations > 0; }

	private:
		void deleteBone(cpBone* bone)
		{
			if (bone->mWeights != nullptr)
				delete[] bone->mWeights;
			delete bone;
		}

		void deleteMesh(cpMesh* mesh)
		{
			if (mesh->mVertices != nullptr)
				delete[] mesh->mVertices;
			if (mesh->mNormals != nullptr)
				delete[] mesh->mNormals;
			for (unsigned int i = 0; i < MAX_TEXTURE_CHANNELS; i++)
			{
				if (mesh->mTextureCoords[i] != nullptr)
					delete[] mesh->mTextureCoords[i];
				if (mesh->mColors[i] != nullptr)
					delete[] mesh->mColors[i];
			}
			if (mesh->mFaces != nullptr)
			{
				for (unsigned int i = 0; i < mesh->mNumFaces; i++)
				{
					if (mesh->mFaces[i].mIndices != nullptr)
						delete[] mesh->mFaces[i].mIndices;
				}
				delete[] mesh->mFaces;
			}
			if (mesh->mBones != nullptr)
			{
				for (unsigned int i = 0; i < mesh->mNumBones; i++)
				{
					if (mesh->mBones[i] != nullptr)
						deleteBone(mesh->mBones[i]);
				}
				delete[] mesh->mBones;
			}
			delete mesh;
		}

		void deleteAnim(cpAnimation* anim)
		{
			if (anim->mChannels == nullptr)
				return;
			for (unsigned int i = 0; i < anim->mNumChannels; i++)
			{
				if (anim->mChannels[i] != nullptr)
				{
					if (anim->mChannels[i]->mPositionKeys != nullptr)
						delete[] anim->mChannels[i]->mPositionKeys;
					if (anim->mChannels[i]->mRotationKeys != nullptr)
						delete[] anim->mChannels[i]->mRotationKeys;
					if (anim->mChannels[i]->mScalingKeys != nullptr)
						delete[] anim->mChannels[i]->mScalingKeys;
					delete anim->mChannels[i];
				}
			}
			delete[] anim->mChannels;
			delete anim;
		}

		void deleteNode(cpNode* node)
		{
			if (node->mChildren != nullptr)
			{
				for (unsigned int i = 0; i < node->mNumChildren; i++)
				{
					if (node->mChildren[i] != nullptr)
						deleteNode(node->mChildren[i]);
				}
				delete[] node->mChildren;
			}
			if (node->mMeshes != nullptr)
				delete[] node->mMeshes;
			delete node;
		}
	};
	/**	Build the scene hierarchy with the data
	**	extracted from the collada file
	*/
	class ColladaSceneBuilder
	{
		/**	Meshes references can have many combinations
		**	with their id, their submesh number and
		**	their associated material.
		**	Multiple nodes can refer to the same mesh
		**	so we need to save those ids to avoid
		**	duplication
		*/
		struct MeshID
		{
			std::string	id;
			size_t		subMesh;
			std::string	material;
			MeshID() = delete;
			MeshID(std::string id, size_t subMesh, std::string material)
				: id(id), subMesh(subMesh), material(material)
			{}
			bool	operator==(const MeshID& ref) const
			{
				return (id == ref.id
					&& subMesh == ref.subMesh
					&& material == ref.material);
			}
		};

	public:
		ColladaSceneBuilder();
		~ColladaSceneBuilder();

		/**	Parse the given file and
		**	construct a scene from it
		*/
		cpScene*
			ReadFile(const std::string& path, unsigned int flags);

	private:

		std::vector<cpMesh*> meshes;
		std::vector<cpAnimation*> anims;
		std::vector<cpTexture*> textures;
		std::vector<MeshID>	meshIDs;
		std::map<std::string, size_t> matIndices;
		std::vector<cpMaterial*> materials;
		std::vector<cpAnimation*> animations;
		std::vector<cpNode*> nodes;

		//	We may fight nodes with no name, use this to assign auto names to them
		unsigned int unamedNodes;

		/**	Save all the nodes ptr in a vector
		**	to run through them easily (without recursion)
		*/
		void
			SaveNodeAsVector(cpNode* node);

		/**	Build a cpNode recursively
		*/
		cpNode*
			BuildNode(ColladaParser& parser, ColladaParser::ColladaNode* node);

		/**	Build all the cpMeshes for a node
		**	by creating them or using already existing ones
		*/
		void
			BuildMeshes(const ColladaParser& parser, const ColladaParser::ColladaNode* node,
				cpNode* newNode);

		/**	Create the array of cpMaterial* from the collada data
		*/
		void
			BuildMaterials(ColladaParser& parser, cpScene* scene);

		/**	Create the array of cpAnimation* from the collada data
		*/
		void
			BuildAnimations(const ColladaParser& parser, cpScene* scene);

		/**	Create the array of cpAnimation* from the collada data
		*/
		void
			BuildAnimation(const ColladaParser& parser, cpScene* scene,
			const ColladaParser::ColladaAnimation& anim, const std::string& prefix);

		/**	Create a cpAnimation
		*/
		void
			CreateAnimation(const ColladaParser& parser, cpScene* scene,
				const ColladaParser::ColladaAnimation& anim,
				const std::string& name);

		/**	Create a new cpMesh ptr with all the collada data
		*/
		cpMesh*
			CreateMesh(const ColladaParser& parser,
				const ColladaParser::ColladaMesh* src,
				const ColladaParser::SubMesh& subMesh,
				const ColladaParser::ColladaController* controller,
				const size_t vertexStart, const size_t faceStart);

		/**	Create a final cpMaterial from a Collada effect
		*/
		cpMaterial*
			CreateMaterial(const ColladaParser& parser, const ColladaParser::ColladaEffect& effect);

		/**	Helper to read a float from a source array with the right accessor
		*/
		float
			ReadFloat(const std::vector<float>& array, const ColladaParser::ColladaAccessor& acc,
			size_t index, size_t offset);

		/**	Helper to read a string from a source array with the right accessor
		*/
		std::string
			ReadString(const std::vector<std::string>& array, const ColladaParser::ColladaAccessor& acc,
			size_t index, size_t offset);

		/**	Search for a cpNode in the scene's hierarchy
		*/
		const cpNode*
			FindCpNode(const cpNode* node, const std::string& name);

		/**	Once the node hierarchy and meshes are built,
		**	resolve bones name's references
		**	(sometimes they're linked to node's id, sometimes sid and sometimes name)
		*/
		void
			ResolveBonesName(const ColladaParser& parser, const cpNode* node);

	};

}
#endif
