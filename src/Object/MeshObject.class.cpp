#include "Object/MeshObject.class.hpp"

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
	MeshObject::MeshObject()
	{
	}

	MeshObject::~MeshObject()
	{
	}

	MeshObject::MeshObject(MeshObject const& meshObject)
	{
		*this = meshObject;
	}

	MeshObject::MeshObject(std::string path)
	{
		loadObject(path);
	}

	MeshObject& MeshObject::operator=(MeshObject const& meshObject)
	{
		this->meshes = meshObject.meshes;
		return *this;
	}

	unsigned int	MeshObject::loadTexture(std::string file, std::string directory)
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

		glGenTextures(1, &id);
		glBindTexture(GL_TEXTURE_2D, id);
		glTexImage2D(GL_TEXTURE_2D, 0, (GLint)format, w, h, 0, format, GL_UNSIGNED_BYTE, img);
		glGenerateMipmap(GL_TEXTURE_2D);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		stbi_image_free(img);

		return id;
	}

	std::vector<Texture>	MeshObject::loadMaterialTextures(aiMaterial* mat,
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

	Mesh	MeshObject::processMesh(aiMesh* mesh, const aiScene* scene)
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

			vector.x = mesh->mNormals[i].x;
			vector.y = mesh->mNormals[i].y;
			vector.z = mesh->mNormals[i].z;
			vertex.norm = vector;

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

		return Mesh(vertices, indices, textures);
	}

	void	MeshObject::processNode(aiNode* node, const aiScene* scene)
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

	void	MeshObject::loadObject(std::string path)
	{
		std::cout << "Loading " << path << "..." << std::endl;
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

	void	MeshObject::draw()
	{
		for (size_t i = 0; i < meshes.size(); i++)
		{
			meshes[i].draw();
		}
	}
}