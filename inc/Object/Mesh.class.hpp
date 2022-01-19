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
		std::vector<std::shared_ptr<Mesh>> const&
			getChildren() const;
		std::string const&
			getName() const;
		mft::vec3 const&
			getColor() const;
		unsigned int const&
				getShader() const;

		// Transform is public so its non-const operations can be called efficiently
		Transform	transform;

		//	Setters

		void
			setName(std::string name);
		void
			setColor(mft::vec3 color);
		void
			setShader(unsigned int shader);
		void
			setShader(GLShaderProgram* shader);

		void
			addTexture(std::shared_ptr < Texture >& text);
		/**	Draw the mesh from the given parent matrix
		**	If a shader is given, the mesh will be drawn with it
		**	instead of the mesh's shader data
		*/
		void
			draw(mft::mat4 parentMat, unsigned int shader = 0);

		void
			addMesh(std::shared_ptr<Mesh> mesh);

	private:
		std::string				name;

		std::shared_ptr<GLMesh>	glMesh;
		std::vector<std::shared_ptr<Mesh>>	children;
		std::shared_ptr<Mesh>	*parent;

		mft::mat4	parentMatrix;
		mft::mat4	transformMatrix;
		mft::mat4	normalMatrix;

		mft::vec3	color;

		unsigned int	shader;

	};

	std::ostream& operator<<(std::ostream& o, Mesh const& mesh);

}

#endif
