#ifndef _MESH_CLASS_H_
# define _MESH_CLASS_H_

#include "GLMesh.class.hpp"
#include "Transform.class.hpp"
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
		std::vector<Mesh> const&
			getChildren() const;
		std::string const&
			getName() const;

		// Transform is public so its non-const operations can be called efficiently
		Transform	transform;

		//	Setters

		void	setName(std::string name);

		void	addTexture(std::shared_ptr < Texture >& text);
		void	draw(GLShaderProgram *shader, mft::mat4 parentMat);

	private:
		std::string				name;

		std::shared_ptr<GLMesh>	glMesh;
		std::vector<Mesh>		children;
		Mesh					*parent;

		mft::mat4	matrix;
	};

	std::ostream& operator<<(std::ostream& o, Mesh const& mesh);

}

#endif
