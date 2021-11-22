#include "Object/MeshData.class.hpp"

#include <string>

namespace notrealengine
{
	//	Constructors

	MeshData::MeshData()
	{
	}

	MeshData::MeshData(MeshData const& meshData)
	{
		// /!\ I don't know if we should copy a reference directly /!\

		*this = meshData;

		/*this->vertices = mesh.vertices;
		this->indices = mesh.indices;
		this->textures = mesh.textures;

		this->name = mesh.name;

		this->VBO = mesh.VBO;
		this->VAO = mesh.VAO;
		this->EBO = mesh.EBO;*/
	}

	MeshData::~MeshData()
	{
	}

	MeshData::MeshData(std::vector<Vertex> vertices,
		std::vector<unsigned int> indices,
		std::vector<Texture> textures)
		: vertices(vertices), indices(indices), textures(textures)
	{
		setup();
	}

	MeshData& MeshData::operator=(MeshData const& meshData)
	{
		this->vertices = meshData.vertices;
		this->indices = meshData.indices;
		this->textures = meshData.textures;

		this->name = meshData.name;
		return *this;
	}

	//	Accessors

	std::vector<Vertex>
		MeshData::getVertices() const
	{
		return vertices;
	}

	std::vector<unsigned int>
		MeshData::getIndices() const
	{
		return indices;
	}

	std::vector<Texture>
		MeshData::getTextures() const
	{
		return textures;
	}

	//	Main functions

	//		Initalize buffers and vertex array for the mesh
	void	MeshData::setup()
	{
	}

	std::ostream& operator<<(std::ostream& o, MeshData const& meshData)
	{
		std::vector<Vertex>			vertices = meshData.getVertices();
		std::vector<unsigned int>	indices = meshData.getIndices();
		std::vector<Texture>		textures = meshData.getTextures();
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
		std::cout << "\t--Textures--" << std::endl;
		for (size_t i = 0; i < textures.size(); i++)
		{
			std::cout << "\t" << textures[i].path << std::endl;
		}
		return o;
	}
}