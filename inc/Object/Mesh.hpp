#ifndef _MESH_HPP_
# define _MESH_HPP_

#include "Object/Vertex.hpp"
#include <vector>

class Mesh
{
	public:
		Mesh();
		Mesh(Mesh const & mesh);
		// Most important constructor, is going to be used
		// to initiate meshes from assimp
		Mesh(std::vector<Vertex> vertices,
			std::vector<unsigned int> indices,
			std::vector<unsigned int> textures);
		~Mesh();
		Mesh & operator=(Mesh const& mesh);

		void	draw() const;

	private:
		std::vector<Vertex>			vertices;
		std::vector<unsigned int>	indices;
		std::vector<unsigned int>	textures;

		std::string	name;

		unsigned int	VAO, VBO, EBO;
		void	setup();
};

	

#endif