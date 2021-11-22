#ifndef _MESH_OBJECT_CLASS_H_
# define _MESH_OBJECT_CLASS_H_

#include "Object/GLMesh.class.hpp"
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

namespace notrealengine
{
	class GLObject
	{
		public:

			std::string	name;

			GLObject();
			GLObject(GLObject const & GLObject);
			GLObject(std::string path);
			~GLObject();
			GLObject& operator=(GLObject const& GLObject);

			void	draw() const;

			//	Accessors

			std::vector<GLMesh>
				getMeshes() const;

		private:
			std::vector<GLMesh>		meshes;
			std::vector<Texture>	loadedTextures;

			std::string	directory;

			//	Object loading
			unsigned int
				loadTexture(std::string file, std::string directory);
			void
				loadObject(std::string path);
			void
				processNode(aiNode* node, const aiScene* scene);
			GLMesh
				processMesh(aiMesh* mesh, const aiScene* scene);
			std::vector<Texture>
				loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
	};

	std::ostream& operator<<(std::ostream& o, GLObject const& obj);
}

#endif