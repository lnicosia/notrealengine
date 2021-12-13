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

	struct Transform
	{
		mft::vec3	pos;
		mft::vec3	rotation;
		mft::vec3	scale;
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
		mft::mat4 const&
			getMatrix() const;

		//	Transforms

		void	update(void);
		void	move(mft::vec3 move);
		void	rotate(mft::vec3 rotation);
		void	scale(mft::vec3 scale);

	private:

		std::vector<Vertex>			vertices;
		std::vector<unsigned int>	indices;

		//	Transforms

		Transform	transform;
		mft::mat4	matrix;

	};

	std::ostream& operator<<(std::ostream& o, MeshData const& meshData);
}



#endif