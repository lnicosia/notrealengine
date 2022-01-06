#include "Object/GLObject.class.hpp"
#include "Object/TextureLoader.class.hpp"
#include "mft/mft.hpp"

//	OpenGL includes
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
#include <windows.h>
#include <glad/glad.h>
#else
# define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>
#endif

#include <fstream>

namespace notrealengine
{

	GLObject::~GLObject()
	{
	}

	GLObject::GLObject(GLObject const& GLObject)
	{
		*this = GLObject;
	}

	GLObject::GLObject(const std::string& path)
		: name("Unkown object"),
		transform{mft::vec3(0, 0, 0),
		mft::vec3(0, 0, 0), mft::vec3(1, 1, 1) },
		directory(""), matrix(), bones(), nbBones(0)
	{
		loadObject(path);
	}

	GLObject::GLObject(std::vector<std::shared_ptr<Mesh>>& meshes)
		: name("Unkown object"),
		transform{ mft::vec3(0, 0, 0),
		mft::vec3(0, 0, 0), mft::vec3(1, 1, 1) },
		directory(""), matrix(), meshes(meshes), bones(), nbBones(0)
	{

	}

	GLObject& GLObject::operator=(GLObject const& GLObject)
	{
		this->meshes = GLObject.meshes;
		return *this;
	}

	//	Texture utility

	void	GLObject::addTexture(unsigned int mesh, std::shared_ptr < Texture >& text)
	{
		if (mesh >= 0 && mesh < meshes.size())
			(*meshes[mesh]).addTexture(text);
		else
			throw std::runtime_error("Mesh index out of bounds");
	}

	// Transforms

	void	GLObject::update(void)
	{
		matrix = mft::mat4();
		matrix *= mft::mat4::scale(transform.scale);
		matrix *= mft::mat4::rotate(transform.rotation.x, mft::vec3(1.0f, 0.0f, 0.0f));
		matrix *= mft::mat4::rotate(transform.rotation.y, mft::vec3(0.0f, 1.0f, 0.0f));
		matrix *= mft::mat4::rotate(transform.rotation.z, mft::vec3(0.0f, 0.0f, 1.0f));
		matrix *= mft::mat4::translate(transform.pos);
		//std::cout << "Object matrix = " << std::endl << matrix << std::endl;
	}

	void	GLObject::move(mft::vec3 move)
	{
		transform.pos = transform.pos + move;
		update();
	}


	void	GLObject::rotate(mft::vec3 rotation)
	{
		transform.rotation = transform.rotation + rotation;
		update();
	}

	void	GLObject::scale(mft::vec3 scale)
	{
		transform.scale = transform.scale + scale;
		update();
	}

	mft::mat4	GLObject::AssimpToMftMatrix(aiMatrix4x4 mat) const
	{
		mft::mat4	res(
			{ mat.a1, mat.a2, mat.a3, mat.a4 },
			{ mat.b1, mat.b2, mat.b3, mat.b4 },
			{ mat.c1, mat.c2, mat.c3, mat.c4 },
			{ mat.d1, mat.d2, mat.d3, mat.d4 });
		return res;
	}

	std::vector<std::shared_ptr<Texture>>	GLObject::loadMaterialTextures(aiMaterial* mat,
		aiTextureType type, std::string typeName, const aiScene *scene)
	{
		std::vector<std::shared_ptr<Texture>>	textures;

		for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
		{
			aiString	str;
			mat->GetTexture(type, i, &str);
			std::string	path = directory + '/' + std::string(str.C_Str());
			const aiTexture* texture;
			if ((texture = scene->GetEmbeddedTexture(str.C_Str())))
			{
				if (texture->mHeight == 0)
					textures.push_back(TextureLoader::loadTexture(path,
						reinterpret_cast<unsigned char*>(texture->pcData),
						texture->mWidth, typeName));
				else
					textures.push_back(TextureLoader::loadTexture(path,
						reinterpret_cast<unsigned char *>(texture->pcData),
						texture->mWidth * texture->mHeight, typeName));
			}
			else
			{
				//std::cout << "Loading " << typeName << " " << str.C_Str() << " from material" << std::endl;
				textures.push_back(TextureLoader::loadTexture(path, typeName));
			}
		}
		return textures;
	}

	void	GLObject::SetVertexBoneData(Vertex& vertex, int id, float weight)
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

	void	GLObject::ExtractBoneInfo(std::vector<Vertex>& vertices, aiMesh* mesh,
		const aiScene* scene)
	{
		//std::cout << "Mesh " << mesh->mName.C_Str() << " has " << mesh->mNumBones << " bones" << std::endl;
		for (int i = 0; i < mesh->mNumBones; i++)
		{
			BoneInfo	bone;
			bone.id = -1;
			std::string	boneName = mesh->mBones[i]->mName.C_Str();
			//std::cout << "Bone " << boneName << std::endl;
			if (bones.find(boneName) == bones.end())
			{
				bone.id = i;
				bone.offset = mft::mat4::inverse(AssimpToMftMatrix(mesh->mBones[i]->mOffsetMatrix));
				bones[boneName] = bone;
				nbBones++;
			}
			else
			{
				bone.id = bones[boneName].id;
			}
			//std::cout << "Id = " << bone.id << std::endl;
			aiVertexWeight *weights = mesh->mBones[i]->mWeights;
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

	std::shared_ptr<Mesh>	GLObject::processMesh(aiMesh* mesh, const aiScene* scene)
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

		if (mesh->mMaterialIndex >= 0)
		{
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
		}

		/*for (int i = 0; i < vertices.size(); i++)
		{
			std::cout << "Vertex " << i << ":" << std::endl;
			std::cout << "Bone " << 0 << ": id = " << vertices[i].boneIDs[0];
			std::cout << ", weight = " << vertices[i].weights[0] << std::endl;
			std::cout << "Bone " << 1 << ": id = " << vertices[i].boneIDs[1];
			std::cout << ", weight = " << vertices[i].weights[1] << std::endl;
			std::cout << "Bone " << 2 << ": id = " << vertices[i].boneIDs[2];
			std::cout << ", weight = " << vertices[i].weights[2] << std::endl;
			std::cout << "Bone " << 3 << ": id = " << vertices[i].boneIDs[3];
			std::cout << ", weight = " << vertices[i].weights[3] << std::endl;
		}*/
		MeshData	data = MeshData(vertices, indices);
		std::shared_ptr<GLMesh>	glMesh(new GLMesh(data, textures));
		return std::shared_ptr<Mesh>(new Mesh(glMesh));
	}

	void	GLObject::processNode(aiNode* node, const aiScene* scene)
	{
		//std::cout << "Node " << node->mName.C_Str() << std::endl;
		for (unsigned int i = 0; i < node->mNumMeshes; i++)
		{
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			// OPTI !! Multiple nodes can refer to the same mesh
			meshes.push_back(processMesh(mesh, scene));
		}
		for (unsigned int i = 0; i < node->mNumChildren; i++)
		{
			processNode(node->mChildren[i], scene);
		}
	}

	void	GLObject::loadObject(std::string path)
	{
		std::cout << "Loading object '" << path << "'..." << std::endl;
		name = path.substr(path.find_last_of('/'), name.size());

		Assimp::Importer	importer;
		const aiScene* scene;
		scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs
			| aiProcess_GenUVCoords);

		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		{
			std::cerr << "assimp: " << importer.GetErrorString() << std::endl;
			return;
		}
		directory = path.substr(0, path.find_last_of('/'));
		processNode(scene->mRootNode, scene);

	}

	//	Drawing functions

	void	GLObject::draw(GLShaderProgram *shader) const
	{
		GLCallThrow(glUseProgram, shader->programID);
		for (size_t i = 0; i < meshes.size(); i++)
		{
			(*meshes[i]).draw(shader, matrix);
		}
	}

	void	GLObject::drawBones(GLShaderProgram* shader, std::shared_ptr<GLMesh> mesh) const
	{
		GLCallThrow(glUseProgram, shader->programID);
		GLCallThrow(glDisable, GL_DEPTH_TEST);
		std::map<std::string, BoneInfo>::const_iterator it;
		for (it = bones.begin(); it != bones.end(); it++)
		{
			Mesh	cube(mesh);
			cube.setColor(mft::vec3(204.0f / 255.0f, 0.0f, 204.0f / 255.0f));
			cube.draw(shader, (*it).second.offset * matrix);
			//return;
		}
		GLCallThrow(glEnable, GL_DEPTH_TEST);
	}

	//	Accessors

	std::vector<std::shared_ptr<Mesh>> const&	GLObject::getMeshes() const
	{
		return meshes;
	}

	mft::mat4 const& GLObject::getMatrix() const
	{
		return matrix;
	}

	Transform const& GLObject::getTransform() const
	{
		return transform;
	}

	std::string const& GLObject::getName() const
	{
		return name;
	}

	int const& GLObject::getNbBones() const
	{
		return nbBones;
	}

	//	Setters

	void	GLObject::setName(std::string name)
	{
		this->name = name;
	}

	std::ostream& operator<<(std::ostream& o, GLObject const& obj)
	{
		std::vector<std::shared_ptr<Mesh>>	meshes = obj.getMeshes();
		std::cout << obj.getName();
		for (size_t i = 0; i < meshes.size(); i++)
		{
			std::cout << "Mesh " << i << ":" << std::endl << *meshes[i];
		}
		return o;
	}
}