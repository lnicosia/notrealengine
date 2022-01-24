#ifndef _GLOBJECT_CLASS_H_
# define _GLOBJECT_CLASS_H_

#include "Object/Mesh.class.hpp"
#include "Object/Texture.class.hpp"

//	Fix for assimp
#undef max
#undef min
#include "assimp/Importer.hpp"
#include "assimp/postprocess.h"
#include "assimp/scene.h"

#include <memory>

namespace notrealengine
{
	class GLObject
	{

		public:

			GLObject() = delete;
			GLObject(GLObject const & GLObject);
			explicit GLObject(std::string path);
			~GLObject();
			GLObject& operator=(GLObject const& GLObject);

			void	draw(GLShaderProgram *shader) const;

			//	Accessors

			[[nodiscard]] std::vector<std::shared_ptr<Mesh>> const&
				getMeshes() const;
			[[nodiscard]] mft::mat4 const&
				getMatrix() const;
			[[nodiscard]] Transform const&
				getTransform() const;
			[[nodiscard]] std::string const&
				getName() const;

			//	Setters

			void	setName(std::string name);

			//	Transforms

			void	update();
			void	move(mft::vec3 move);
			void	rotate(mft::vec3 rotation);
			void	scale(mft::vec3 scale);

			//	Texture utility

			void	addTexture(unsigned int mesh, std::shared_ptr < Texture> & text);

		private:
			std::string	name;

			std::vector<std::shared_ptr<Mesh>>	meshes;

			std::string	directory;

			//	Transforms

			Transform	transform;
			mft::mat4	matrix;

			//	Object loading
			void
				loadObject(const std::string& path);
			void
				processNode(aiNode* node, const aiScene* scene);
			std::shared_ptr<Mesh>
				processMesh(aiMesh* mesh, const aiScene* scene);
			std::vector<std::shared_ptr<Texture>>
				loadMaterialTextures(aiMaterial* mat, aiTextureType type, const std::string& typeName);
	};

	std::ostream& operator<<(std::ostream& o, GLObject const& obj);
} // namespace notrealengine

#endif