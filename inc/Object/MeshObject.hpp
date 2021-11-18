#ifndef _MESH_OBJECT_H_
# define _MESH_OBJECT_H_

#include "Object/Mesh.hpp"

class MeshObject
{
	public:
		MeshObject();
		MeshObject(MeshObject const & meshObject);
		MeshObject(std::string path);
		~MeshObject();
		MeshObject& operator=(MeshObject const& meshObject);

	private:
		std::vector<Mesh>	meshes;

		std::string	name;

		//	Object loading
		void	loadObject(std::string path);
		//void	processNode(aiNode* node, const aiScene* scene);
		//void	processMesh(aiNode* node, const aiScene* scene);
};

#endif