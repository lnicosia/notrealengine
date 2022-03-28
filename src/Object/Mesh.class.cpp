#include "Object/Mesh.class.hpp"
#include "GLContext.class.hpp"

namespace notrealengine
{
	Mesh::Mesh(std::shared_ptr<GLMesh> const& glMesh)
		: name(glMesh->getName()), transform(),
		glMesh(glMesh), animDirty(false),
		parentMatrix(), transformMatrix(), animMatrix(), normalMatrix(),
		shader(GLContext::getShader("default")->programID),
		color(mft::vec3(0.239f, 0.282f, 0.286f))
	{

	}

	Mesh::~Mesh()
	{

	}

	//	Accessors

	std::shared_ptr<GLMesh> const& Mesh::getGLMesh() const
	{
		return glMesh;
	}

	std::vector<std::shared_ptr<Mesh>> const& Mesh::getChildren() const
	{
		return children;
	}

	std::string const& Mesh::getName() const
	{
		return name;
	}

	mft::vec3 const& Mesh::getColor() const
	{
		return color;
	}

	unsigned int const& Mesh::getShader() const
	{
		return shader;
	}

	//	Setters

	void	Mesh::setName(std::string name)
	{
		this->name = name;
	}

	void	Mesh::setColor(mft::vec3 color)
	{
		this->color = color;
	}

	void 	Mesh::setShader(unsigned int shader)
	{
		this->shader = shader;
	}

	void 	Mesh::setShader(GLShaderProgram* shader)
	{
		this->shader = shader->programID;
	}

	void 	Mesh::setAnimMatrix(const mft::mat4& mat)
	{
		this->animMatrix = mat;
		this->animDirty = true;
	}

	void	Mesh::draw(const mft::mat4& parentMat, unsigned int overrideShader)
	{
		unsigned int finalShader = overrideShader == 0 ? this->shader : overrideShader;
		//	Recompute transform matrix if parent's one has changed
		if (parentMat != this->parentMatrix || this->transform.isDirty() || this->animDirty)
		{
			this->transformMatrix = this->transform.getMatrix() * parentMat * this->animMatrix;
			mft::mat4 tmp = mft::mat4::transpose(mft::mat4::inverse(this->transformMatrix));
			this->normalMatrix = mft::mat3(
				{ tmp[0][0], tmp[0][1], tmp[0][2] },
				{ tmp[1][0], tmp[1][1], tmp[1][2] },
				{ tmp[2][0], tmp[2][1], tmp[2][2] }
				);
			this->parentMatrix = parentMat;
			this->animDirty = false;
		}
		//std::cout << parentMat << " * " << matrix << " = " << tmp << std::endl;
		GLCallThrow(glUseProgram, finalShader);
		GLint location = GLCallThrow(glGetUniformLocation, finalShader, "baseColor");
		GLCallThrow(glUniform3f, location, color.x, color.y, color.z);
		//std::cout << "Drawing mesh " << name << " with matrix " << transformMatrix << std::endl;
		glMesh->draw(finalShader, this->transformMatrix, normalMatrix);
		for (auto child: children)
		{
			child->draw(this->transformMatrix, finalShader);
		}
	}

	void	Mesh::addTexture(std::shared_ptr < Texture >& text)
	{
		(*glMesh).addTexture(text);
	}

	void	Mesh::addMesh(std::shared_ptr<Mesh> mesh)
	{
		children.push_back(mesh);
	}

	std::ostream& operator<<(std::ostream& o, Mesh const& mesh)
	{
		return o;
	}
}
