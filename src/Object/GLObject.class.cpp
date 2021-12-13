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

namespace notrealengine
{

	GLObject::~GLObject()
	{
	}

	GLObject::GLObject(GLObject const& GLObject)
	{
		*this = GLObject;
	}

	GLObject::GLObject(std::string path) : transform{ mft::vec3(0, 0, 0),
		mft::vec3(0, 0, 0), mft::vec3(1, 1, 1) }, matrix()
	{
		loadObject(path);
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

	std::vector<std::shared_ptr<Texture>>	GLObject::loadMaterialTextures(aiMaterial* mat,
		aiTextureType type, std::string typeName)
	{
		std::vector<std::shared_ptr<Texture>>	textures;

		for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
		{
			aiString	str;
			mat->GetTexture(type, i, &str);
			//std::cout << "Loading " << typeName << " " << str.C_Str() << " from material" << std::endl;
			std::string		path = directory + '/' + std::string(str.C_Str());
			textures.push_back(TextureLoader::loadTexture(path, typeName));
		}
		return textures;
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
				aiTextureType_DIFFUSE, "texture_diffuse");
			textures.insert(
				textures.end(),
				std::make_move_iterator(diffuseMaps.begin()),
				std::make_move_iterator(diffuseMaps.end()));
			std::vector< std::shared_ptr<Texture>> specularMaps = loadMaterialTextures(material,
				aiTextureType_SPECULAR, "texture_specular");
			textures.insert(
				textures.end(),
				std::make_move_iterator(specularMaps.begin()),
				std::make_move_iterator(specularMaps.end()));
		}

		MeshData	data = MeshData(vertices, indices);
		std::shared_ptr<GLMesh>	glMesh(new GLMesh(data, textures));
		return std::shared_ptr<Mesh>(new Mesh(glMesh));
	}

	void	GLObject::processNode(aiNode* node, const aiScene* scene)
	{
		for (unsigned int i = 0; i < node->mNumMeshes; i++)
		{
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
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

	void	GLObject::draw(GLShaderProgram *shader) const
	{
		GLCallThrow(glUseProgram, shader->programID);
		//GLCallThrow(glUniformMatrix4fv, GLCallThrow(glGetUniformLocation, shader->programID, "model"), 1, GL_TRUE, &matrix[0][0]);
		for (size_t i = 0; i < meshes.size(); i++)
		{
			(*meshes[i]).draw(shader, matrix);
		}
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