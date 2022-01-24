#include "Object/GLMesh.class.hpp"

//	OpenGL includes
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
	#include <windows.h>
	#include <glad/glad.h>
#else
	# define GL_GLEXT_PROTOTYPES
	#include <GL/gl.h>
#endif
#include <string>
#include <utility>
#include <utility>

namespace notrealengine
{
	//	Constructors

	GLMesh::GLMesh(GLMesh && GLMesh)  noexcept :
		name(std::move(GLMesh.name)),
		textures(std::move(GLMesh.textures)),
		VAO(std::exchange(GLMesh.VAO, 0)),
		VBO(std::exchange(GLMesh.VBO, 0)),
		EBO(std::exchange(GLMesh.EBO, 0)),
		nbIndices(std::exchange(GLMesh.nbIndices, 0)),
		polygonMode(std::exchange(GLMesh.polygonMode, 0))
	{

	}

	GLMesh::~GLMesh()
	{
		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(1, &VBO);
		glDeleteBuffers(1, &EBO);
	}

	GLMesh::GLMesh(MeshData const & data, std::vector<std::shared_ptr<Texture>> textures)
		: 
		textures(std::move(std::move(textures))),
		VAO(0), VBO(0), EBO(0),
		polygonMode(GL_FILL),
		nbIndices(0)
	{
		setup(data);
	}

	GLMesh& GLMesh::operator=(GLMesh && GLMesh)
 noexcept 	{
		this->name = std::move(GLMesh.name);

		this->textures = std::move(GLMesh.textures);

		this->VBO = std::exchange(GLMesh.VBO, 0);
		this->VAO = std::exchange(GLMesh.VAO, 0);
		this->EBO = std::exchange(GLMesh.EBO, 0);

		this->polygonMode = std::exchange(GLMesh.polygonMode, 0);

		this->nbIndices = std::exchange(GLMesh.nbIndices, 0);
		return *this;
	}

	//	Accessors

	std::vector<std::shared_ptr<Texture>> const &	GLMesh::getTextures() const
	{
		return textures;
	}

	unsigned int const &	GLMesh::getVAO() const
	{
		return VAO;
	}

	unsigned int const& GLMesh::getVBO() const
	{
		return VBO;
	}

	unsigned int const& GLMesh::getEBO() const
	{
		return EBO;
	}

	unsigned int const& GLMesh::getPolygonMode() const
	{
		return polygonMode;
	}

	std::string const& GLMesh::getName() const
	{
		return name;
	}

	//	Setters

	void	GLMesh::setName(std::string name)
	{
		this->name = std::move(name);
	}

	//	Texture utility

	void	GLMesh::addTexture(std::shared_ptr < Texture >& text)
	{
		textures.push_back(std::move(text));
	}

	//	Main functions

	void	GLMesh::draw(GLShaderProgram *shader, mft::mat4 transform) const
	{
		unsigned int	diffuse = 0;
		unsigned int	specular = 0;
		GLCallThrow(glUniformMatrix4fv, GLCallThrow(glGetUniformLocation, shader->programID, "model"), 1, GL_TRUE, static_cast<float*>(transform));
		for (size_t i = 0; i < textures.size(); i++)
		{
			GLCallThrow(glActiveTexture, GL_TEXTURE0 + (unsigned int)i);

			std::string	nb;
			std::string	name = (*textures[i]).getType();
			if (name == "texture_diffuse") {
				nb = std::to_string(diffuse);
			} else if (name == "texture_specular") {
				nb = std::to_string(specular);
}
			GLCallThrow(glBindTexture, GL_TEXTURE_2D, (*textures[i]).getId());
			GLCallThrow(glUniform1f, GLCallThrow(glGetUniformLocation, shader->programID, ("material." + name + nb).c_str()), i);
		}
		GLCallThrow(glActiveTexture, GL_TEXTURE0);
		GLCallThrow(glBindVertexArray, VAO);
		GLCallThrow(glPolygonMode, GL_FRONT_AND_BACK, polygonMode);
		GLCallThrow(glDrawElements, GL_TRIANGLES, nbIndices, GL_UNSIGNED_INT, 0);
		GLCallThrow(glBindVertexArray, 0);
	}

	//		Initalize buffers and vertex array for the GLMesh
	void	GLMesh::setup(MeshData const& data)
	{
		GLCallThrow(glGenVertexArrays, 1, &VAO);
		GLCallThrow(glGenBuffers, 1, &VBO);
		GLCallThrow(glGenBuffers, 1, &EBO);

		GLCallThrow(glBindVertexArray, VAO);

		GLCallThrow(glBindBuffer, GL_ARRAY_BUFFER, VBO);
		GLCallThrow(glBufferData, GL_ARRAY_BUFFER,
			data.getVertices().size() * sizeof(Vertex),
			&data.getVertices()[0], GL_STATIC_DRAW);

		GLCallThrow(glBindBuffer, GL_ELEMENT_ARRAY_BUFFER, EBO);
		GLCallThrow(glBufferData, GL_ELEMENT_ARRAY_BUFFER,
			data.getIndices().size() * sizeof(unsigned int),
			data.getIndices().data(), GL_STATIC_DRAW);

		nbIndices = data.getIndices().size();

		GLCallThrow(glVertexAttribPointer,
			0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)nullptr);
		GLCallThrow(glEnableVertexAttribArray, 0);
		GLCallThrow(glVertexAttribPointer,
			1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
		GLCallThrow(glEnableVertexAttribArray, 1);
		GLCallThrow(glVertexAttribPointer,
			2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
		GLCallThrow(glEnableVertexAttribArray, 2);

		GLCallThrow(glBindVertexArray, 0);
	}

	std::ostream& operator<<(std::ostream& o, GLMesh const&  /*GLMesh*/)
	{
		/*std::vector<Vertex>			vertices = GLMesh.getData().getVertices();
		std::vector<unsigned int>	indices = GLMesh.getData().getIndices();
		std::vector<Texture>		textures = GLMesh.getData().getTextures();
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
		std::cout << "\tVAO = " << GLMesh.getVAO() << std::endl;
		std::cout << "\tVBO = " << GLMesh.getVBO() << std::endl;
		std::cout << "\tEBO = " << GLMesh.getEBO() << std::endl;*/
		return o;
	}
} // namespace notrealengine