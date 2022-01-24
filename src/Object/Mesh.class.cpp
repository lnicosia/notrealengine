#include "Object/Mesh.class.hpp"

#include <utility>

#include <utility>

namespace notrealengine
{
	Mesh::Mesh(std::shared_ptr<GLMesh>  glMesh)
		: transform{ mft::vec3(0, 0, 0), mft::vec3(0, 0, 0), mft::vec3(1, 1, 1)},
		 glMesh(std::move(glMesh))
	{

	}

	Mesh::~Mesh()
	= default;

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

	mft::mat4 const& Mesh::getMatrix() const
	{
		return matrix;
	}

	Transform const& Mesh::getTransform() const
	{
		return transform;
	}

	std::string const& Mesh::getName() const
	{
		return name;
	}

	//	Setters

	void	Mesh::setName(std::string name)
	{
		this->name = std::move(name);
	}

	// Transforms

	void	Mesh::update()
	{
		matrix = mft::mat4();
		matrix *= mft::mat4::scale(transform.scale);
		matrix *= mft::mat4::rotate(transform.rotation.x, mft::vec3(1.0F, 0.0F, 0.0F));
		matrix *= mft::mat4::rotate(transform.rotation.y, mft::vec3(0.0F, 1.0F, 0.0F));
		matrix *= mft::mat4::rotate(transform.rotation.z, mft::vec3(0.0F, 0.0F, 1.0F));
		matrix *= mft::mat4::translate(transform.pos);
		//std::cout << "Mesh matrix = " << std::endl << matrix << std::endl;
	}

	void	Mesh::move(mft::vec3 move)
	{
		transform.pos = transform.pos + move;
		update();
	}


	void	Mesh::rotate(mft::vec3 rotation)
	{
		transform.rotation = transform.rotation + rotation;
		update();
	}

	void	Mesh::scale(mft::vec3 scale)
	{
		transform.scale = transform.scale + scale;
		update();
	}

	void	Mesh::draw(GLShaderProgram* shader, mft::mat4 parentMat)
	{
		mft::mat4	tmp = parentMat * matrix;
		//std::cout << parentMat << " * " << matrix << " = " << tmp << std::endl;
		(*glMesh).draw(shader, tmp);
		for (auto child: children)
		{
			child.draw(shader, tmp);
		}
	}

	std::ostream& operator<<(std::ostream& o, Mesh const&  /*mesh*/)
	{
		return o;
	}
} // namespace notrealengine
