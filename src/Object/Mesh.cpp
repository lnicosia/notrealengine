#include "Object/Mesh.hpp"

Mesh::Mesh(): VAO(0), VBO(0), EBO(0)
{
}

Mesh::Mesh(Mesh const& mesh)
{
	// /!\ I don't know if we should copy a reference directly /!\

	*this = mesh;

	/*this->vertices = mesh.vertices;
	this->indices = mesh.indices;
	this->textures = mesh.textures;

	this->name = mesh.name;

	this->VBO = mesh.VBO;
	this->VAO = mesh.VAO;
	this->EBO = mesh.EBO;*/
}

Mesh::~Mesh()
{
}

Mesh::Mesh(std::vector<Vertex> vertices,
	std::vector<unsigned int> indices,
	std::vector<unsigned int> textures)
	: vertices(vertices), indices(indices), textures(textures),
		VAO(0), VBO(0), EBO(0)
{
	setup();
}

Mesh& Mesh::operator=(Mesh const& mesh)
{
	this->vertices = mesh.vertices;
	this->indices = mesh.indices;
	this->textures = mesh.textures;

	this->name = mesh.name;

	this->VBO = mesh.VBO;
	this->VAO = mesh.VAO;
	this->EBO = mesh.EBO;
	return *this;
}

void	Mesh::draw() const
{

}

void	Mesh::setup()
{

}