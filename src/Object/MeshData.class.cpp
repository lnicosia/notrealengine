#include "Object/MeshData.class.hpp"
#include <cmath>
#include <string>
#include <utility>
#include <utility>

namespace notrealengine
{

	MeshData::MeshData(MeshData const& meshData)
	{
		*this = meshData;
	}

	MeshData::~MeshData()
	= default;

	MeshData::MeshData(std::vector<Vertex> vertices,
		std::vector<unsigned int> indices)
		: vertices(std::move(std::move(vertices))), indices(std::move(std::move(indices))),
			transform { mft::vec3(0, 0, 0), mft::vec3(0, 0, 0), mft::vec3(1, 1, 1) }
			
	{
	}

	MeshData& MeshData::operator=(MeshData const& meshData)
	= default;

	//	Accessors

	std::vector<Vertex> const&	MeshData::getVertices() const
	{
		return vertices;
	}

	std::vector<unsigned int> const&	MeshData::getIndices() const
	{
		return indices;
	}

	mft::mat4 const& MeshData::getMatrix() const
	{
		return matrix;
	}

	void	MeshData::update()
	{
		matrix = mft::mat4();
		matrix *= mft::mat4::scale(transform.scale);
		matrix *= mft::mat4::rotate(transform.rotation.x, mft::vec3(1.0F, 0.0F, 0.0F));
		matrix *= mft::mat4::rotate(transform.rotation.y, mft::vec3(0.0F, 1.0F, 0.0F));
		matrix *= mft::mat4::rotate(transform.rotation.z, mft::vec3(0.0F, 0.0F, 1.0F));
		matrix *= mft::mat4::translate(transform.pos);
		//std::cout << "Mesh matrix = " << std::endl << matrix << std::endl;
	}

	void	MeshData::move(mft::vec3 move)
	{
		transform.pos = transform.pos + move;
		update();
	}

	void	MeshData::rotate(mft::vec3 rotation)
	{
		transform.rotation = transform.rotation + rotation;
		update();
	}

	void	MeshData::scale(mft::vec3 scale)
	{
		transform.scale = transform.scale + scale;
		update();
	}

	//	Main functions

	std::ostream& operator<<(std::ostream& o, MeshData const& meshData)
	{
		std::vector<Vertex>			vertices = meshData.getVertices();
		std::vector<unsigned int>	indices = meshData.getIndices();
		std::cout << "\t--Vertices--" << std::endl;
		for (auto & vertice : vertices)
		{
			std::cout << "\t" << vertice.pos << vertice.norm << vertice.uv << std::endl;
		}
		std::cout << "\t--Indices--" << std::endl;
		for (unsigned int indice : indices)
		{
			std::cout << "\t" << indice << std::endl;
		}
		return o;
	}
} // namespace notrealengine