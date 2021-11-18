#ifndef _MESH_OBJECT_H_
# define _MESH_OBJECT_H_

#include "Object/Mesh.hpp"
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

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
		std::vector<Mesh>	meshes;

		std::string	name;

		//	Object loading
		void	loadObject(std::string path);
		void	processNode(aiNode* node, const aiScene* scene);
		Mesh	processMesh(aiMesh* mesh, const aiScene* scene);
};

#endif