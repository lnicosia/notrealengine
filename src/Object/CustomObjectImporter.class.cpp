#include "Object/CustomObjectImporter.class.hpp"
#include "Object/AssetManager.class.hpp"


namespace notrealengine
{
	CustomObjectImporter::CustomObjectImporter(): ObjectImporter()
	{
	}

	CustomObjectImporter::~CustomObjectImporter()
	{
	}

	void	CustomObjectImporter::ReadFile(const std::string& path, unsigned int flags)
	{
		this->path = path;
		ColladaSceneBuilder importer;
		const cpScene* scene;
		scene = importer.ReadFile(path, flags);
		if (!scene || !scene->mRootNode)
		{
			std::cerr << "Collada parser failed to import object" << std::endl;
			return;
		}
		this->directory = path.substr(0, path.find_last_of('/'));
		processNode(scene->mRootNode, scene);
		processNodeBones(scene->mRootNode, scene, mft::mat4());

		delete scene;
		//readMissingBones(scene);

	}

	void	CustomObjectImporter::SetVertexBoneData(Vertex& vertex, int id, float weight)
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

	void	CustomObjectImporter::ExtractBoneInfo(std::vector<Vertex>& vertices, cpMesh* mesh,
		const cpScene* scene)
	{
		//std::cout << "Mesh " << mesh->mName << " has " << mesh->mNumBones << " bones" << std::endl;
		for (int i = 0; i < mesh->mNumBones; i++)
		{
			BoneInfo	bone;
			bone.id = -1;
			std::string	boneName = mesh->mBones[i]->mName;
			bone.name = boneName;
			if (this->bones.find(boneName) == bones.end())
			{
				bone.id = nbBones;
				bone.offsetMatrix = mesh->mBones[i]->mOffsetMatrix;
				bone.modelMatrix = mft::mat4::inverse(bone.offsetMatrix);
				bone.localMatrix = mft::mat4();
				bone.originalMatrix = mft::mat4();
				this->bones[boneName] = bone;
				this->nbBones++;
			}
			else
			{
				bone.id = this-> bones[boneName].id;
			}
			//std::cout << "Bone " << boneName <<  " id = " << bone.id << std::endl;
			cpVertexWeight* weights = mesh->mBones[i]->mWeights;
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

	std::shared_ptr<Mesh>	CustomObjectImporter::processMesh(cpMesh* mesh, const cpScene* scene)
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

			//std::cout << "Vertex: " << vector << std::endl;

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
			//std::cout << "Vertex: pos = " << vertex.pos << ", norm = " << vertex.norm;
			//std::cout << ", uv = " << vertex.uv << std::endl;
		}
		ExtractBoneInfo(vertices, mesh, scene);

		//	Indices

		for (unsigned int i = 0; i < mesh->mNumFaces; i++)
		{
			cpFace	face = mesh->mFaces[i];
			for (unsigned int j = 0; j < face.mNumIndices; j++)
			{
				indices.push_back(face.mIndices[j]);
			}
		}

		//std::cout << "Retrieving material " << mesh->mMaterialIndex << " for mesh " << mesh->mName << std::endl;
		cpMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
		std::vector<std::shared_ptr<Texture>> diffuseMaps = loadMaterialTextures(material,
			diffuse, "texture_diffuse", scene);
		textures.insert(
			textures.end(),
			std::make_move_iterator(diffuseMaps.begin()),
			std::make_move_iterator(diffuseMaps.end()));
		std::vector< std::shared_ptr<Texture>> specularMaps = loadMaterialTextures(material,
			specular, "texture_specular", scene);
		textures.insert(
			textures.end(),
			std::make_move_iterator(specularMaps.begin()),
			std::make_move_iterator(specularMaps.end()));

		MeshData	data = MeshData(vertices, indices);
		std::shared_ptr<GLMesh>	glMesh(new GLMesh(data, textures));
		std::shared_ptr<Mesh>	res(new Mesh(glMesh));
		res->setName(mesh->mName);
		return res;
	}

	void	CustomObjectImporter::processNodeBones(cpNode* node, const cpScene* scene, const mft::mat4& parentMat)
	{
		mft::mat4	transform =  parentMat * node->mTransformation;
		std::string name(node->mName);
		if (bones.contains(name))
		{
			bones[name].originalMatrix = transform;
			bones[name].localMatrix = transform * bones[name].offsetMatrix;
		}
		for (unsigned int i = 0; i < node->mNumChildren; i++)
		{
			processNodeBones(node->mChildren[i], scene, transform);
		}
	}

	void	CustomObjectImporter::processNode(cpNode* node, const cpScene* scene)
	{
		//std::cout << "Reading node " << node->mName << std::endl;
		for (unsigned int i = 0; i < node->mNumMeshes; i++)
		{
			cpMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			//std::cout << "Reading mesh " << mesh->mName << std::endl;
			// OPTI !! Multiple nodes can refer to the same mesh
			meshes.push_back(processMesh(mesh, scene));
		}
		for (unsigned int i = 0; i < node->mNumChildren; i++)
		{
			processNode(node->mChildren[i], scene);
		}
	}

	void CustomObjectImporter::readMissingBones(const cpScene* scene)
	{
		for (unsigned int i = 0; i < scene->mNumAnimations; i++)
		{
			cpAnimation* anim = scene->mAnimations[i];
			if (anim == nullptr)
				continue;
			for (unsigned int j = 0; j < anim->mNumChannels; j++)
			{
				cpNodeAnim* bone = anim->mChannels[i];
				std::string name = bone->mNodeName;
				if (this->bones.find(name) == this->bones.end())
				{
					std::cout << "Found missing bone '" << name << "'" << std::endl;
					this->bones[name].id = nbBones;
					//this->bones[name].fromParentMatrix = CustomToMftMatrix(bone->mTransformation)
					nbBones++;
				}
			}
		}
	}

	std::vector<std::shared_ptr<Texture>>
		CustomObjectImporter::loadMaterialTextures(cpMaterial* mat,
			cpTextureType type, std::string typeName, const cpScene* scene)
	{
		std::vector<std::shared_ptr<Texture>>	textures;

		for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
		{
			std::string	str;
			mat->GetTexture(type, i, str);
			std::filesystem::path textPath(str);
			textPath.make_preferred();
			std::string	path = directory + '/' + textPath.string();
			/*const cpTexture* texture;
			if ((texture = scene->GetEmbeddedTexture(str)))
			{
				if (texture->mHeight == 0)
					textures.push_back(AssetManager::getInstance().loadAsset<Texture>(this->path,
						reinterpret_cast<unsigned char*>(texture->pcData),
						texture->mWidth, typeName));
				else
					textures.push_back(AssetManager::getInstance().loadAsset<Texture>(this->path,
						reinterpret_cast<unsigned char*>(texture->pcData),
						texture->mWidth * texture->mHeight, typeName));
			}
			else
			{*/
				//std::cout << "Loading " << typeName << " " << str.C_Str() << " from material" << std::endl;
			textures.push_back(AssetManager::getInstance().loadAsset<Texture>(path, typeName));
			//}
		}
		return textures;
	}
}
