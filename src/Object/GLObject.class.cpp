#include "Object/GLObject.class.hpp"
#include "mft/mft.hpp"

//	OpenGL includes
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
#include <windows.h>
#include <glad/glad.h>
#else
# define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>
#endif

//	Image loading library
#ifdef __unix__
	#pragma GCC diagnostic push
	#pragma GCC diagnostic ignored "-Wconversion"
#endif
#define STB_IMAGE_IMPLEMENTATION
#include "../lib/stb_image.h"
#ifdef __unix__
	#pragma GCC diagnostic pop
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
		mft::vec3(0, 0, 0), mft::vec3(0, 0, 0) }
	{
		loadObject(path);
	}

	GLObject& GLObject::operator=(GLObject const& GLObject)
	{
		this->meshes = GLObject.meshes;
		return *this;
	}

	// Transforms

	void	GLObject::update(void)
	{
		/*matrix = mft::mat4(1);
		matrix = mft::translate(matrix, transform.pos);
		matrix = mft::rotate(matrix, mft::radians(1), transform.rotation);
		matrix = mft::scale(matrix, transform.scale);*/
	}

	void	GLObject::move(mft::vec3 move)
	{
		transform.pos = transform.pos + move;
	}


	void	GLObject::rotate(mft::vec3 rotation)
	{
		transform.rotation = transform.rotation + rotation;
	}

	void	GLObject::scale(mft::vec3 scale)
	{
		transform.scale = transform.scale + scale;
	}

	unsigned int	GLObject::loadTexture(std::string file, std::string directory)
	{
		unsigned int	id = 0;
		std::string		path = file + '/' + directory;

		int	w, h, nChannels;
		unsigned char* img = stbi_load(path.c_str(), &w, &h, &nChannels, 0);
		if (!img)
		{
			std::cerr << "Failed to load " + path << std::endl;
			stbi_image_free(img);
			return id;
		}
		GLenum	format;
		if (nChannels == 1)
			format = GL_RED;
		else if (nChannels == 3)
			format = GL_RGB;
		else if (nChannels == 4)
			format = GL_RGBA;

		GLCallThrow(glGenTextures, 1, &id);
		GLCallThrow(glBindTexture, GL_TEXTURE_2D, id);
		GLCallThrow(glTexImage2D, GL_TEXTURE_2D, 0, (GLint)format, w, h, 0, format, GL_UNSIGNED_BYTE, img);
		GLCallThrow(glGenerateMipmap, GL_TEXTURE_2D);
		GLCallThrow(glTexParameteri, GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		GLCallThrow(glTexParameteri, GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		GLCallThrow(glTexParameteri, GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		GLCallThrow(glTexParameteri, GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		stbi_image_free(img);

		return id;
	}

	std::vector<Texture>	GLObject::loadMaterialTextures(aiMaterial* mat,
		aiTextureType type, std::string typeName)
	{
		std::vector<Texture>	textures;

		for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
		{
			aiString	str;
			mat->GetTexture(type, i, &str);
			bool	exists = false;
			for (unsigned int j = 0; j < loadedTextures.size(); j++)
			{
				if (std::strcmp(loadedTextures[i].path.c_str(), str.C_Str()) == 0)
				{
					textures.push_back(loadedTextures[i]);
					exists = true;
					break;
				}
			}
			if (exists)
				continue;
			Texture		texture;
			texture.id = loadTexture(str.C_Str(), directory);
			texture.type = typeName;
			texture.path = str.C_Str();
			textures.push_back(texture);
			loadedTextures.push_back(texture);
		}
		return textures;
	}

	GLMesh	GLObject::processMesh(aiMesh* mesh, const aiScene* scene)
	{
		std::vector<Vertex>			vertices;
		std::vector<unsigned int>	indices;
		std::vector<Texture>		textures;

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

			if (mesh->mTextureCoords[0])
			{
				mft::vec2	uv;
				uv.x = mesh->mTextureCoords[0][i].x;
				uv.y = mesh->mTextureCoords[0][i].y;
			}
			else
				vertex.uv = mft::vec2();
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
			std::vector<Texture> diffuseMaps = loadMaterialTextures(material,
				aiTextureType_DIFFUSE, "texture_diffuse");
			textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
			std::vector<Texture> specularMaps = loadMaterialTextures(material,
				aiTextureType_SPECULAR, "texture_specular");
			textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
		}

		//return Mesh(vertices, indices, textures);
		MeshData	data = MeshData(vertices, indices, textures);
		return GLMesh(data);
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
		std::cout << "Loading '" << path << "'..." << std::endl;
		name = path.substr(path.find_last_of('/'), name.size());

		Assimp::Importer	importer;
		const aiScene* scene;
		scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

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
		//glUniformMatrix4fv(glGetUniformLocation(shader->programID, "model"), 1, GL_TRUE, matrix);
		for (size_t i = 0; i < meshes.size(); i++)
		{
			meshes[i].draw(shader);
		}
	}

	std::vector<GLMesh>	GLObject::getMeshes() const
	{
		return meshes;
	}

	std::ostream& operator<<(std::ostream& o, GLObject const& obj)
	{
		std::vector<GLMesh>	meshes = obj.getMeshes();
		std::cout << obj.name;
		for (size_t i = 0; i < meshes.size(); i++)
		{
			std::cout << "Mesh " << i << ":" << std::endl << meshes[i];
		}
		return o;
	}
}