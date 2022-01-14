#ifndef _GLOBJECT_CLASS_H_
# define _GLOBJECT_CLASS_H_

#include "Object/Mesh.class.hpp"
#include "Object/Texture.class.hpp"
#include "Object/Transform.class.hpp"
#include "Object/Asset.class.hpp"

//	Fix for assimp
#undef max
#undef min
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

#include <memory>
#include <map>

namespace notrealengine
{
	struct BoneInfo
	{
		int	id;
		mft::mat4	offset;
	};

	class GLObject: public Asset
	{

		public:

			GLObject() = delete;
			GLObject(GLObject const & GLObject);
			GLObject(const std::string& path);
			GLObject(std::vector<std::shared_ptr<Mesh>>& meshes);
			~GLObject();
			GLObject& operator=(GLObject const& GLObject);

			//	Draw functions

			void
				draw(GLShaderProgram *shader) const;

			//	Render all the object's bones
			void
				drawBones(GLShaderProgram* shader, std::shared_ptr<GLMesh> mesh) const;

			//	Accessors

			std::vector<std::shared_ptr<Mesh>> const&
				getMeshes() const;
			int const&
				getNbBones() const;

			virtual const std::string
				getAssetType() const;

			//	Setters

			void	setName(std::string name);

			//	Texture utility

			void	addTexture(unsigned int mesh, std::shared_ptr < Texture> & text);

			// Transform is public so its non-const operations can be called efficiently
			Transform	transform;

		private:

			std::vector<std::shared_ptr<Mesh>>	meshes;

			std::string	directory;

			std::map<std::string, BoneInfo>	bones;
			int	nbBones;

			//	Object loading
			void
				SetVertexBoneData(Vertex& vertex, int id, float weight);
			mft::mat4
				AssimpToMftMatrix(aiMatrix4x4 mat) const;
			void
				ExtractBoneInfo(std::vector<Vertex>& vertices, aiMesh* mesh, const aiScene* scene);
			void
				loadObject(std::string path);
			void
				processNode(aiNode* node, const aiScene* scene);
			std::shared_ptr<Mesh>
				processMesh(aiMesh* mesh, const aiScene* scene);
			std::vector<std::shared_ptr<Texture>>
				loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName, const aiScene *scene);
	};

	std::ostream& operator<<(std::ostream& o, GLObject const& obj);
}

#endif
