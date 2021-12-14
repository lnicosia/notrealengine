#ifndef _MESH_CLASS_H_
# define _MESH_CLASS_H_

#include "GLMesh.class.hpp"
#include <memory>

namespace notrealengine
{
	class Mesh
	{
	public:
		Mesh() = delete;
		Mesh(std::shared_ptr<GLMesh> const& glMesh);
		~Mesh();

		//	Accessors

		std::shared_ptr<GLMesh> const&
			getGLMesh() const;
		std::vector<std::shared_ptr<Mesh>> const&
			getChildren() const;
		mft::mat4 const&
			getMatrix() const;
		Transform const&
			getTransform() const;
		std::string const&
			getName() const;

		//	Setters

		void	setName(std::string name);

		//	Tranform

		void	update();
		void	move(mft::vec3 move);
		void	rotate(mft::vec3 rotation);
		void	scale(mft::vec3 scale);

		void	addTexture(std::shared_ptr < Texture >& text);
		void	draw(GLShaderProgram *shader, mft::mat4 parentMat);

		void	addMesh(std::shared_ptr<Mesh> mesh);

	private:
		std::string				name;

		std::shared_ptr<GLMesh>	glMesh;
		std::vector<std::shared_ptr<Mesh>>	children;
		std::shared_ptr<Mesh>	*parent;

		Transform	transform;
		mft::mat4	matrix;
	};

	std::ostream& operator<<(std::ostream& o, Mesh const& mesh);

}

#endif