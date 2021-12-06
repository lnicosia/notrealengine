#ifndef _MESH_DATA_CLASS_H_
# define _MESH_DATA_CLASS_H_

#include "mft/mft.hpp"
#include <vector>

namespace notrealengine
{
	struct Vertex
	{
		mft::vec3	pos;
		mft::vec3	norm;
		mft::vec2	uv;
	};

	struct Texture
	{
		unsigned int	glId;
		std::string		type;
		std::string		path;
	};

	struct Transform
	{
		mft::vec3	pos;
		mft::vec3	rotation;
		mft::vec3	scale;
	};

	class MeshData
	{

	public:

		std::string	name;

		MeshData();
		MeshData(MeshData const& meshData);
		// Most important constructor, is going to be used
		// to initiate meshes from assimp
		MeshData(std::vector<Vertex> vertices,
			std::vector<unsigned int> indices,
			std::vector<Texture> textures);
		~MeshData();
		MeshData& operator=(MeshData const& meshData);

		// Accessors

		std::vector<Vertex>			getVertices() const;
		std::vector<unsigned int>	getIndices() const;
		std::vector<Texture>		getTextures() const;

		mft::mat4					getMatrix() const;

		//	Texture utility

		void	addTexture(Texture text);

		//	Transforms

		void	update(void);
		void	move(mft::vec3 move);
		void	rotate(mft::vec3 rotation);
		void	scale(mft::vec3 scale);

	private:
		std::vector<Vertex>			vertices;
		std::vector<unsigned int>	indices;
		std::vector<Texture>		textures;

		//	Transforms

		Transform	transform;
		mft::mat4	matrix;

		//		Initalize buffers and vertex array for the mesh
		//		Should not be called outside of the class scope
		void	setup();
	};

	std::ostream& operator<<(std::ostream& o, MeshData const& meshData);
}



#endif