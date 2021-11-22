#ifndef _MESH_DATA_CLASS_H_
# define _MESH_DATA_CLASS_H_

#include "mft/mft.hpp"
#include <vector>

namespace notrealengine
{
	struct Vertex
	{
		mft::vec3	pos;
		mft::vec3	norm;
		mft::vec2	uv;
	};

	struct Texture
	{
		unsigned int	id;
		std::string		type;
		std::string		path;
	};

	class MeshData
	{

	public:

		std::string	name;

		MeshData();
		MeshData(MeshData const& meshData);
		// Most important constructor, is going to be used
		// to initiate meshes from assimp
		MeshData(std::vector<Vertex> vertices,
			std::vector<unsigned int> indices,
			std::vector<Texture> textures);
		~MeshData();
		MeshData& operator=(MeshData const& meshData);

		// Accessors

		std::vector<Vertex>			getVertices() const;
		std::vector<unsigned int>	getIndices() const;
		std::vector<Texture>		getTextures() const;

	private:
		std::vector<Vertex>			vertices;
		std::vector<unsigned int>	indices;
		std::vector<Texture>		textures;

		//		Initalize buffers and vertex array for the mesh
		//		Should not be called outside of the class scope
		void	setup();
	};

	std::ostream& operator<<(std::ostream& o, MeshData const& meshData);
}



#endif