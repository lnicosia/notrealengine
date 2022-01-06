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

	class MeshData
	{

	public:

		MeshData() = delete;
		MeshData(MeshData const& meshData);
		// Most important constructor, is going to be used
		// to initiate meshes from assimp
		MeshData(std::vector<Vertex> vertices,
			std::vector<unsigned int> indices);
		~MeshData();
		MeshData& operator=(MeshData const& meshData);

		// Accessors

		std::vector<Vertex> const&
			getVertices() const;
		std::vector<unsigned int> const&
			getIndices() const;

	private:

		std::vector<Vertex>			vertices;
		std::vector<unsigned int>	indices;
	};

	std::ostream& operator<<(std::ostream& o, MeshData const& meshData);
}



#endif
