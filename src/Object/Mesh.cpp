#include "Object/Mesh.hpp"
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
	#include <windows.h>
	#include <glad/glad.h>
#else
	# define GL_GLEXT_PROTOTYPES
	#include <GL/gl.h>
#endif
#include <string>

//	Constructors

Mesh::Mesh(): VAO(0), VBO(0), EBO(0), polygon_mode(GL_FILL)
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
	std::vector<Texture> textures)
	: vertices(vertices), indices(indices), textures(textures),
		VAO(0), VBO(0), EBO(0), polygon_mode(GL_FILL)
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

	this->polygon_mode = mesh.polygon_mode;
	return *this;
}

//	Accessors

unsigned int const	Mesh::getVAO()
{
	return VAO;
}

unsigned int const	Mesh::getVBO()
{
	return VBO;
}

unsigned int const	Mesh::getEBO()
{
	return EBO;
}

unsigned int const	Mesh::getPolygonMode()
{
	return polygon_mode;
}

void	Mesh::draw() const
{
	unsigned int	diffuse = 0;
	unsigned int	specular = 0;
	for (size_t i = 0; i < textures.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + (unsigned int)i);

		std::string	nb;
		if (textures[i].type == "texture_diffuse")
			nb = std::to_string(diffuse);
		else if (textures[i].type == "texture_specular")
			nb = std::to_string(specular);
		glBindTexture(GL_TEXTURE_2D, textures[i].id);
	}
	glPolygonMode(GL_FRONT_AND_BACK, polygon_mode);
	glDrawElements(GL_TRIANGLES, (int)indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

//		Initalize buffers and vertex array for the mesh
void	Mesh::setup()
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, (long)vertices.size() * ((long)sizeof(Vertex)), &vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, (long)indices.size() * ((long)sizeof(unsigned int)),
		&indices[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(6 * sizeof(float)));

	glBindVertexArray(0);
}