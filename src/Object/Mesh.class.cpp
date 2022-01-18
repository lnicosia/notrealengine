#include "Object/Mesh.class.hpp"
#include "GLContext.class.hpp"

namespace notrealengine
{
	Mesh::Mesh(std::shared_ptr<GLMesh> const& glMesh)
		: transform(),
		glMesh(glMesh),
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

	void 	Mesh::setShader(unsigned int shader) {
		this->shader = shader;
	}

	void 	Mesh::setShader(GLShaderProgram* shader) {
		this->shader = shader->programID;
	}

	// Transforms

	/*void	Mesh::update(void)
	{
		matrix = mft::mat4();
		matrix *= mft::mat4::scale(transform.scale);
		matrix *= mft::mat4::rotate(transform.rotation.x, mft::vec3(1.0f, 0.0f, 0.0f));
		matrix *= mft::mat4::rotate(transform.rotation.y, mft::vec3(0.0f, 1.0f, 0.0f));
		matrix *= mft::mat4::rotate(transform.rotation.z, mft::vec3(0.0f, 0.0f, 1.0f));
		matrix *= mft::mat4::translate(transform.pos);
		//std::cout << "Mesh matrix = " << std::endl << matrix << std::endl;
	}

	void	Mesh::move(mft::vec3 move)
	{
		transform.pos = transform.pos + move;
		//std::cout << name << " pos = " << std::endl << transform.pos << std::endl;
		update();
	}


	void	Mesh::rotate(mft::vec3 rotation)
	{
		transform.rotation = transform.rotation + rotation;
		//std::cout << name << " rotation = " << std::endl << transform.rotation << std::endl;
		update();
	}

	void	Mesh::scale(mft::vec3 scale)
	{
		transform.scale = transform.scale + scale;
		//std::cout << name << " scale = " << std::endl << transform.scale << std::endl;
		update();
	}*/

	void	Mesh::draw(mft::mat4 parentMat, unsigned int overrideShader) const
	{
		unsigned int finalShader = overrideShader == 0 ? this->shader : overrideShader;
		mft::mat4	transformMatrix = parentMat * transform.getMatrix();
		//std::cout << parentMat << " * " << matrix << " = " << tmp << std::endl;
		GLCallThrow(glUseProgram, finalShader);
		GLCallThrow(glUniform3f, GLCallThrow(glGetUniformLocation, finalShader, "baseColor"), color.x, color.y, color.z);
		//std::cout << "Drawing mesh " << name << " with matrix " << transformMatrix << std::endl;
		glMesh->draw(finalShader, transformMatrix);
		for (auto child: children)
		{
			child->draw(transformMatrix);
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
