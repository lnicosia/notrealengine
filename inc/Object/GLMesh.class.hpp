#ifndef _GLMESH_CLASS_H_
# define _GLMESH_CLASS_H_

#include "mft/mft.hpp"
#include "Object/MeshData.class.hpp"
#include "Object/Texture.class.hpp"
#include "notrealengine/GLShaderProgram.class.hpp"
#include <vector>

namespace notrealengine
{

	class GLMesh
	{

	public:

		GLMesh() = delete;
		GLMesh(GLMesh const & mesh) = delete;
		GLMesh(GLMesh && mesh);
		GLMesh(MeshData const& data, std::vector<std::shared_ptr<Texture>> textures);
		~GLMesh();
		GLMesh& operator=(GLMesh const& mesh) = delete;
		GLMesh& operator=(GLMesh && mesh);

		//	Accessors

		std::vector<std::shared_ptr<Texture>> const&
			getTextures() const;
		std::string const&
			getName() const;
		unsigned int const&
			getVAO() const;
		unsigned int const&
			getVBO() const;
		unsigned int const&
			getEBO() const;
		unsigned int const&
			getPolygonMode() const;


		//	Setters

		void
			setName(std::string name);

		//	Texture utility

		void
			addTexture(std::shared_ptr<Texture>& text);

		void
			draw(unsigned int shader, const mft::mat4& transform, const mft::mat3& normalMatrix) const;

	private:
		std::string	name;

		std::vector<std::shared_ptr<Texture>>	textures;

		unsigned int	VAO, VBO, EBO;

		unsigned int	polygonMode;

		size_t			nbIndices;

		//		Initalize buffers and vertex array for the mesh
		//		Should not be called outside of the class scope
		void	setup(MeshData const& data);
	};

	std::ostream& operator<<(std::ostream& o, GLMesh const& mesh);
}



#endif
