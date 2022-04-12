#ifndef _ASSIMP_OBJECT_IMPORTER_CLASS_H_
#define _ASSIMP_OBJECT_IMPORTER_CLASS_H_

#include "Object/ObjectImporter.class.hpp"

#ifdef USING_EXTERNAL_LIBS
//	Fix for assimp
#undef max
#undef min
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

namespace notrealengine
{
	class AssimpObjectImporter : public ObjectImporter
	{
	public:
		AssimpObjectImporter();
		~AssimpObjectImporter();

		void
			ReadFile(const std::string& path, unsigned int flags) override;
	private:
		void
			SetVertexBoneData(Vertex& vertex, int id, float weight);
		void
			ExtractBoneInfo(std::vector<Vertex>& vertices, aiMesh* mesh, const aiScene* scene);
		void
			processNode(aiNode* node, const aiScene* scene);
		void
			processNodeBones(aiNode* node, const aiScene* scene, const mft::mat4& parentMat);
		std::shared_ptr<Mesh>
			processMesh(aiMesh* mesh, const aiScene* scene);
		std::vector<std::shared_ptr<Texture>>
			loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName, const aiScene* scene);
		/**	Sometimes bones are missing from the original skeleton but can be
		**	found in the object's animations
		*/
		void
			readMissingBones(const aiScene* scene);
	};
}
#endif // !USING_EXTERNAL_LIBS

#endif // !_ASSIMP_OBJECT_IMPORTER_CLASS_H_
