#include "Object/Mesh.class.hpp"

namespace notrealengine
{
	Mesh::Mesh(std::shared_ptr<GLMesh> const& glMesh):
		transform(),
		glMesh(glMesh)
	{

	}

	Mesh::~Mesh()
	{

	}

	void	Mesh::addTexture(std::shared_ptr < Texture >& text)
	{
		(*glMesh).addTexture(text);
	}

	//	Accessors

	std::shared_ptr<GLMesh> const& Mesh::getGLMesh() const
	{
		return glMesh;
	}

	std::vector<Mesh> const& Mesh::getChildren() const
	{
		return children;
	}

	std::string const& Mesh::getName() const
	{
		return name;
	}

	//	Setters

	void	Mesh::setName(std::string name)
	{
		this->name = name;
	}

	// Transforms

	void	Mesh::draw(GLShaderProgram* shader, mft::mat4 parentMat)
	{
		mft::mat4	transformMatrix = parentMat * transform.getMatrix();
		//std::cout << parentMat << " * " << matrix << " = " << tmp << std::endl;
		(*glMesh).draw(shader, transformMatrix);
		for (auto child: children)
		{
			child.draw(shader, transformMatrix);
		}
	}

	std::ostream& operator<<(std::ostream& o, Mesh const& mesh)
	{
		return o;
	}
}
