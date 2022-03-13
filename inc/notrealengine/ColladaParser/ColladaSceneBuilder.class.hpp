#ifndef _COLLADA_SCENE_BUILDER_CLASS_H_
#define _COLLADA_SCENE_BUILDER_CLASS_H_

#include "ColladaParser/ColladaParser.class.hpp"

namespace notrealengine
{
	enum cpTextureType
	{
		diffuse,
		specular,
		normal
	};
	struct cpTexture
	{
		unsigned int	mHeight;
	};
	struct cpMaterial
	{
		unsigned int
			GetTextureCount(cpTextureType type);
		void
			GetTexture(cpTextureType type, unsigned int index, std::string& str);
	};
	struct cpNodeAnim
	{
		std::string mNodeName;
	};
	struct cpAnimation
	{
		cpNodeAnim**	mChannels;
		unsigned int	mNumChannels;
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
	};
	struct cpFace
	{
		unsigned int* mIndices;
		unsigned int  mNumIndices;
	};
	struct cpMesh
	{
		mft::vec3* mVertices;
		mft::vec3* mNormals;
		mft::vec3* mTextureCoords[MAX_TEXTURE_COORDINATES];
		unsigned int mNumUVComponents[MAX_TEXTURE_COORDINATES];
		mft::vec4* mColors[MAX_TEXTURE_COORDINATES];

		cpBone** mBones;
		cpFace*	mFaces;

		std::string		mName;

		unsigned int	mNumVertices;
		unsigned int	mNumFaces;
		unsigned int	mMaterialIndex;
		unsigned int	mNumBones;
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

		mft::mat4		mTranformation;
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

		/**	Build a cpNode recursively
		*/
		cpNode*
			BuildNode(ColladaParser& parser, ColladaParser::ColladaNode* node);

		/**	Build all the cpMeshes for a node
		**	by creating them or using already existing ones
		*/
		void
			BuildMeshes(ColladaParser& parser, ColladaParser::ColladaNode* node,
				cpNode* newNode);

		/**	Create the array of cpMaterial* from the collada data
		*/
		void
			BuildMaterials(ColladaParser& parser, cpScene* scene);

		/**	Create a new cpMesh ptr with all the collada data
		*/
		cpMesh*
			CreateMesh(ColladaParser& parser,
				const ColladaParser::ColladaMesh* src,
				const ColladaParser::SubMesh& subMesh,
				const size_t vertexStart, const size_t faceStart);
	};

}
#endif
