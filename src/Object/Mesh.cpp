#include "Object/Mesh.hpp"
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
	#include <windows.h>
	#include <glad/glad.h>
#else
	# define GL_GLEXT_PROTOTYPES
	#include <GL/gl.h>
#endif

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
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int),
		&indices[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(6 * sizeof(float)));

	glBindVertexArray(0);
}