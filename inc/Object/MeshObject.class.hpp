#ifndef _MESH_OBJECT_CLASS_H_
# define _MESH_OBJECT_CLASS_H_

#include "Object/Mesh.class.hpp"
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

namespace notrealengine
{
	class MeshObject
	{
		public:
			MeshObject();
			MeshObject(MeshObject const & meshObject);
			MeshObject(std::string path);
			~MeshObject();
			MeshObject& operator=(MeshObject const& meshObject);

			void	draw();

		private:
			std::vector<Mesh>		meshes;
			std::vector<Texture>	loadedTextures;

			std::string	name;
			std::string	directory;

			//	Object loading
			unsigned int
				loadTexture(std::string file, std::string directory);
			void
				loadObject(std::string path);
			void
				processNode(aiNode* node, const aiScene* scene);
			Mesh
				processMesh(aiMesh* mesh, const aiScene* scene);
			std::vector<Texture>
				loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
	};
}

#endif