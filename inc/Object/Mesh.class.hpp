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
		explicit Mesh(std::shared_ptr<GLMesh>  glMesh);
		~Mesh();

		//	Accessors

		[[nodiscard]] std::shared_ptr<GLMesh> const&
			getGLMesh() const;
		[[nodiscard]] std::vector<Mesh> const&
			getChildren() const;
		[[nodiscard]] mft::mat4 const&
			getMatrix() const;
		[[nodiscard]] Transform const&
			getTransform() const;
		[[nodiscard]] std::string const&
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

	private:
		std::string				name;

		std::shared_ptr<GLMesh>	glMesh;
		std::vector<Mesh>		children;
		Mesh					*parent{};

		Transform	transform;
		mft::mat4	matrix;
	};

	std::ostream& operator<<(std::ostream& o, Mesh const& mesh);

} // namespace notrealengine

#endif