#include "Object/MeshData.class.hpp"
#include <math.h>
#include <string>

namespace notrealengine
{
	//	Constructors

	MeshData::MeshData() 
		: vertices(), indices(), textures(),
			transform{ mft::vec3(0, 0, 0),
			mft::vec3(0, 0, 0), mft::vec3(1, 1, 1) }, matrix()
	{
	}

	MeshData::MeshData(MeshData const& meshData)
	{
		*this = meshData;
	}

	MeshData::~MeshData()
	{
	}

	MeshData::MeshData(std::vector<Vertex> vertices,
		std::vector<unsigned int> indices,
		std::vector<Texture> textures)
		: vertices(vertices), indices(indices), textures(textures),
			transform { mft::vec3(0, 0, 0), mft::vec3(0, 0, 0), mft::vec3(1, 1, 1) },
			matrix()
	{
		setup();
	}

	MeshData& MeshData::operator=(MeshData const& meshData)
	{
		this->vertices = meshData.vertices;
		this->indices = meshData.indices;
		this->textures = meshData.textures;

		this->name = meshData.name;

		this->transform = meshData.transform;
		this->matrix = meshData.matrix;
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

	mft::mat4	MeshData::getMatrix() const
	{
		return matrix;
	}

	//	Texture utility

	void	MeshData::addTexture(Texture text)
	{
		textures.push_back(text);
	}

	void	MeshData::update(void)
	{
		matrix = mft::mat4();
		matrix *= mft::mat4::scale(transform.scale);
		matrix *= mft::mat4::rotate(transform.rotation.x, mft::vec3(1.0f, 0.0f, 0.0f));
		matrix *= mft::mat4::rotate(transform.rotation.y, mft::vec3(0.0f, 1.0f, 0.0f));
		matrix *= mft::mat4::rotate(transform.rotation.z, mft::vec3(0.0f, 0.0f, 1.0f));
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