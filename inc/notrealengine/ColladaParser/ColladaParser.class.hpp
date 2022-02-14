#ifndef _COLLADA_PARSER_CLASS_H_
#define	_COLLADA_PARSER_CLASS_H_

#include "LXML/LXML.class.hpp"
#include "mft/mft.hpp"
#include <string>

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

	};
	struct cpMaterial
	{
		unsigned int
			GetTextureCount(cpTextureType type);
		void
			GetTexture(cpTextureType type, unsigned int index, std::string& str);
	};
	struct cpAnimation
	{

	};
	struct cpMesh
	{
		mft::vec3* mVertices;
		mft::vec3* mNormals;
		mft::vec3* mTextureCoords;

		std::string		mName;

		unsigned int	mNumVertices;
		unsigned int	mNumFaces;
		unsigned int	mMaterialIndex;
	};
	struct cpNode
	{
		cpNode** mChildren;
		cpNode* mParent;
		unsigned int* mMeshes;

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

	class ColladaParser
	{
	public:
		ColladaParser();
		~ColladaParser();

		cpScene*
			ReadFile(const std::string& path, unsigned int flags);
	private:

		/**	Retrieve scene geometry from the file
		*/
		void
			ReadGeometry(const lxml::Importer& parser);

		/**	Read mesh data
		**	@return The read mesh data or nullptr if invalid
		*/
		cpMesh*
			ReadMesh(const lxml::Tag& geometry, const lxml::Tag& mesh);

		/**	Retrieve an int from an Attribute vector
		**	Throws if overflow
		*/
		int
			GetIntAttribute(const std::vector<lxml::Attribute>& attributes, const std::string& name);

		/**	Retrieve an int from an Attribute vector
		**	Throws if overflow
		*/
		unsigned long
			GetUIntAttribute(const std::vector<lxml::Attribute>& attributes, const std::string& name);


		/**	Search for an attribute name in a tag
		**	Set value to the attribute's value if found
		**	@return True if the attribute was found, false otherwise
		*/
		bool
			GetAttribute(const lxml::Tag& tag, const std::string& attribute,
				std::string& value);

		cpScene* scene;
	};


}

#endif // !_COLLADA_PARSER_CLASS_H_
