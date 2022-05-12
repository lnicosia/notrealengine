#ifndef _CUSTOM_OBJECT_IMPORTER_CLASS_H_
#define _CUSTOM_OBJECT_IMPORTER_CLASS_H_

#include "Object/ObjectImporter.class.hpp"
#include "ColladaParser/ColladaSceneBuilder.class.hpp"

namespace notrealengine
{
	class CustomObjectImporter : public ObjectImporter
	{
	public:
		CustomObjectImporter();
		~CustomObjectImporter();

		bool
			ReadFile(const std::string& path, unsigned int flags) override;
	private:
		void
			SetVertexBoneData(Vertex& vertex, int id, float weight);
		void
			ExtractBoneInfo(std::vector<Vertex>& vertices, cpMesh* mesh, const cpScene* scene);
		void
			processNode(cpNode* node, const cpScene* scene);
		void
			processNodeBones(cpNode* node, const cpScene* scene, const mft::mat4& parentMat);
		std::shared_ptr<Mesh>
			processMesh(cpMesh* mesh, const cpScene* scene);
		std::vector<std::shared_ptr<Texture>>
			loadMaterialTextures(cpMaterial* mat, cpTextureType type, std::string typeName, const cpScene* scene);
		/**	Sometimes bones are missing from the original skeleton but can be
		**	found in the object's animations
		*/
		void
			readMissingBones(const cpScene* scene);

	};
}

#endif // !_CUSTOM_OBJECT_IMPORTER_CLASS_H_
