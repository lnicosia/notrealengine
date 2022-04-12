#include "Object/AssimpObjectImporter.class.hpp"
#include "Object/AssimpHelpers.hpp"
#include "Object/AssetManager.class.hpp"

namespace notrealengine
{
	AssimpObjectImporter::AssimpObjectImporter(): ObjectImporter()
	{
	}

	AssimpObjectImporter::~AssimpObjectImporter()
	{
	}

	void	AssimpObjectImporter::ReadFile(const std::string& path, unsigned int flags)
	{
		this->path = path;
		Assimp::Importer	importer;
		const aiScene* scene;
		if (path.substr(path.find_last_of('.')) == ".dae" || path.substr(path.find_last_of('.')) == ".obj")
			scene = importer.ReadFile(path, aiProcess_Triangulate
				| aiProcess_GenUVCoords);
		else
			scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs
				| aiProcess_GenUVCoords);

		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		{
			std::cerr << "assimp: " << importer.GetErrorString() << std::endl;
			return;
		}
		this->directory = path.substr(0, path.find_last_of('/'));
		processNode(scene->mRootNode, scene);
		processNodeBones(scene->mRootNode, scene, mft::mat4());

		//readMissingBones(scene);

	}

	void	AssimpObjectImporter::SetVertexBoneData(Vertex& vertex, int id, float weight)
	{
		for (int i = 0; i < MAX_BONE_INFLUENCE; i++)
		{
			if (vertex.boneIDs[i] == -1)
			{
				vertex.boneIDs[i] = id;
				vertex.weights[i] = weight;
				//std::cout << "COUCOU" << std::endl;
				break;
			}
			//std::cout << "\tid = " << vertex.boneIDs[i] << ", weight = ";
			//std::cout << vertex.weights[i] << std::endl;
		}
	}

	void	AssimpObjectImporter::ExtractBoneInfo(std::vector<Vertex>& vertices, aiMesh* mesh,
		const aiScene* scene)
	{
		//std::cout << "Mesh " << mesh->mName.C_Str() << " has " << mesh->mNumBones << " bones" << std::endl;
		for (int i = 0; i < mesh->mNumBones; i++)
		{
			BoneInfo	bone;
			bone.id = -1;
			std::string	boneName = mesh->mBones[i]->mName.C_Str();
			bone.name = boneName;
			if (this->bones.find(boneName) == bones.end())
			{
				bone.id = nbBones;
				bone.offsetMatrix = AssimpToMftMatrix(mesh->mBones[i]->mOffsetMatrix);
				this->bones[boneName] = bone;
				this->nbBones++;
			}
			else
			{
				bone.id = this->bones[boneName].id;
			}
			//std::cout << "Bone " << boneName <<  " id = " << bone.id << std::endl;
			aiVertexWeight* weights = mesh->mBones[i]->mWeights;
			int	nbWeights = mesh->mBones[i]->mNumWeights;
			for (int j = 0; j < nbWeights; j++)
			{
				if (weights[j].mVertexId >= vertices.size()
					|| weights[j].mWeight == 0)
					continue;
				//std::cout << "Vertex { " << weights[j].mVertexId << " } has the following bones:" << std::endl;
				SetVertexBoneData(vertices[weights[j].mVertexId], bone.id, weights[j].mWeight);
			}
		}
	}

	std::shared_ptr<Mesh>	AssimpObjectImporter::processMesh(aiMesh* mesh, const aiScene* scene)
	{
		std::vector<Vertex>						vertices;
		std::vector<unsigned int>				indices;
		std::vector<std::shared_ptr<Texture>>	textures;

		//	Vertices

		for (unsigned int i = 0; i < mesh->mNumVertices; i++)
		{
			Vertex	vertex;

			mft::vec3	vector;
			vector.x = mesh->mVertices[i].x;
			vector.y = mesh->mVertices[i].y;
			vector.z = mesh->mVertices[i].z;

			//	Get the range of each axis coordinate
			//	to scale the object to fit in our engine unit scale
			//	after parsing

			if (this->isRangeInit == false)
			{
				this->isRangeInit = true;
				this->max = vector;
				this->min = vector;
			}

			if (vector.x > this->max.x)
				this->max.x = vector.x;
			if (vector.y > this->max.y)
				this->max.y = vector.y;
			if (vector.z > this->max.z)
				this->max.z = vector.z;

			if (vector.x < this->min.x)
				this->min.x = vector.x;
			if (vector.y < this->min.y)
				this->min.y = vector.y;
			if (vector.z < this->min.z)
				this->min.z = vector.z;
			vertex.pos = vector;

			if (mesh->mNormals)
			{
				vector.x = mesh->mNormals[i].x;
				vector.y = mesh->mNormals[i].y;
				vector.z = mesh->mNormals[i].z;
				vertex.norm = vector;
			}
			else
			{
				vertex.norm = mft::vec3();
			}
			if (mesh->mTextureCoords[0] != NULL)
			{
				vertex.uv.x = mesh->mTextureCoords[0][i].x;
				vertex.uv.y = mesh->mTextureCoords[0][i].y;
			}
			else
			{
				vertex.uv = mft::vec2();
			}
			vertices.push_back(vertex);
		}
		ExtractBoneInfo(vertices, mesh, scene);

		//	Indices

		for (unsigned int i = 0; i < mesh->mNumFaces; i++)
		{
			aiFace	face = mesh->mFaces[i];
			for (unsigned int j = 0; j < face.mNumIndices; j++)
			{
				indices.push_back(face.mIndices[j]);
			}
		}

		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
		std::vector<std::shared_ptr<Texture>> diffuseMaps = loadMaterialTextures(material,
			aiTextureType_DIFFUSE, "texture_diffuse", scene);
		textures.insert(
			textures.end(),
			std::make_move_iterator(diffuseMaps.begin()),
			std::make_move_iterator(diffuseMaps.end()));
		std::vector< std::shared_ptr<Texture>> specularMaps = loadMaterialTextures(material,
			aiTextureType_SPECULAR, "texture_specular", scene);
		textures.insert(
			textures.end(),
			std::make_move_iterator(specularMaps.begin()),
			std::make_move_iterator(specularMaps.end()));

		MeshData	data = MeshData(vertices, indices);
		std::shared_ptr<GLMesh>	glMesh(new GLMesh(data, textures));
		std::shared_ptr<Mesh>	res(new Mesh(glMesh));
		res->setName(mesh->mName.C_Str());
		return res;
	}

	void	AssimpObjectImporter::processNodeBones(aiNode* node, const aiScene* scene, const mft::mat4& parentMat)
	{
		mft::mat4	transform =  parentMat * AssimpToMftMatrix(node->mTransformation);
		std::string name(node->mName.data);
		std::map<std::string, BoneInfo>::iterator it =
			this->bones.find(name);
		if (it != this->bones.end())
		{
			BoneInfo& bone = it->second;
			bone.originalMatrix = transform;
			bone.modelMatrix = transform;
			bone.localMatrix = transform * bone.offsetMatrix;
		}
		for (unsigned int i = 0; i < node->mNumChildren; i++)
		{
			processNodeBones(node->mChildren[i], scene, transform);
		}
	}

	void	AssimpObjectImporter::processNode(aiNode* node, const aiScene* scene)
	{
		//std::cout << "Reading node " << node->mName.C_Str() << std::endl;
		for (unsigned int i = 0; i < node->mNumMeshes; i++)
		{
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			//std::cout << "Reading mesh " << mesh->mName.C_Str() << std::endl;
			// OPTI !! Multiple nodes can refer to the same mesh
			meshes.push_back(processMesh(mesh, scene));
		}
		for (unsigned int i = 0; i < node->mNumChildren; i++)
		{
			processNode(node->mChildren[i], scene);
		}
	}

	void AssimpObjectImporter::readMissingBones(const aiScene* scene)
	{
		for (unsigned int i = 0; i < scene->mNumAnimations; i++)
		{
			aiAnimation* anim = scene->mAnimations[i];
			if (anim == nullptr)
				continue;
			for (unsigned int j = 0; j < anim->mNumChannels; j++)
			{
				aiNodeAnim* bone = anim->mChannels[i];
				std::string name = bone->mNodeName.data;
				if (this->bones.find(name) == this->bones.end())
				{
					std::cout << "Found missing bone '" << name << "'" << std::endl;
					this->bones[name].id = nbBones;
					//this->bones[name].fromParentMatrix = AssimpToMftMatrix(bone->mTransformation)
					this->nbBones++;
				}
			}
		}
	}

	std::vector<std::shared_ptr<Texture>>
		AssimpObjectImporter::loadMaterialTextures(aiMaterial* mat,
		aiTextureType type, std::string typeName, const aiScene* scene)
	{
		std::vector<std::shared_ptr<Texture>>	textures;

		for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
		{
			aiString	str;
			mat->GetTexture(type, i, &str);
			std::filesystem::path textPath(str.C_Str());
			textPath.make_preferred();
			std::string	path = directory + '/' + textPath.string();
			const aiTexture* texture;
			if ((texture = scene->GetEmbeddedTexture(str.C_Str())))
			{
				if (texture->mHeight == 0)
					textures.push_back(AssetManager::getInstance().loadAsset<Texture>(this->path,
						reinterpret_cast<unsigned char*>(texture->pcData),
						texture->mWidth, typeName));
				else
						textures.push_back(AssetManager::getInstance().loadAsset<Texture>(this->path,
						reinterpret_cast<unsigned char *>(texture->pcData),
						texture->mWidth * texture->mHeight, typeName));
			}
			else
			{
				textures.push_back(AssetManager::getInstance().loadAsset<Texture>(path, typeName));
			}
		}
		return textures;
	}
}
