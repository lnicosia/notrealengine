#ifndef _MESH_CLASS_H_
# define _MESH_CLASS_H_

#include "mft/mft.hpp"
#include "Object/MeshData.class.hpp"
#include "notrealengine/GLShaderProgram.class.hpp"
#include <vector>

namespace notrealengine
{

	class GLMesh
	{

	public:

		std::string	name;

		GLMesh();
		// Most important constructor, is going to be used
		// to initiate meshes from assimp
		GLMesh(GLMesh const & mesh);
		GLMesh(MeshData const& data);
		~GLMesh();
		GLMesh& operator=(GLMesh const& mesh);

		void	draw(GLShaderProgram *shader) const;

		//	Accessors

		MeshData const	getData() const;

		unsigned int const	getVAO() const;
		unsigned int const	getVBO() const;
		unsigned int const	getEBO() const;

		unsigned int const	getPolygonMode() const;

		//	Texture utility

		void	addTexture(Texture text);

		//	Transforms

		void	update(void);
		void	move(mft::vec3 move);
		void	rotate(mft::vec3 rotation);
		void	scale(mft::vec3 scale);

	private:
		MeshData		data;

		unsigned int	VAO, VBO, EBO;

		unsigned int	polygon_mode;

		//		Initalize buffers and vertex array for the mesh
		//		Should not be called outside of the class scope
		void	setup();
	};

	std::ostream& operator<<(std::ostream& o, GLMesh const& mesh);
}

	

#endif