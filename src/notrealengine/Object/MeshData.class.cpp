#include "Object/MeshData.class.hpp"
# define _USE_MATH_DEFINES_
#include <math.h>
#include <string>

namespace notrealengine
{

	MeshData::MeshData(MeshData const& meshData)
	{
		*this = meshData;
	}

	MeshData::~MeshData()
	{
	}

	MeshData::MeshData(std::vector<Vertex> vertices,
		std::vector<unsigned int> indices):
		vertices(vertices), indices(indices)
	{
	}

	MeshData& MeshData::operator=(MeshData const& meshData)
	{
		this->vertices = meshData.vertices;
		this->indices = meshData.indices;

		return *this;
	}

	//	Accessors

	std::vector<Vertex> const&	MeshData::getVertices() const
	{
		return vertices;
	}

	std::vector<unsigned int> const&	MeshData::getIndices() const
	{
		return indices;
	}

	//	Main functions

	std::ostream& operator<<(std::ostream& o, MeshData const& meshData)
	{
		std::vector<Vertex>			vertices = meshData.getVertices();
		std::vector<unsigned int>	indices = meshData.getIndices();
		std::cout << "\t--Vertices--" << std::endl;
		for (size_t i = 0; i < vertices.size(); i++)
		{
			std::cout << "\t" << vertices[i].pos << vertices[i].norm << vertices[i].uv << std::endl;
		}
		std::cout << "\t--Indices--" << std::endl;
		for (size_t i = 0; i < indices.size(); i++)
		{
			std::cout << "\t" << indices[i] << std::endl;
		}
		return o;
	}
}
