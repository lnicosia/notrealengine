#include "Object/MeshObject.hpp"
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
#include <windows.h>
#include <glad/glad.h>
#else
# define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>
#endif

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

MeshObject&	MeshObject::operator=(MeshObject const& meshObject)
{
	this->meshes = meshObject.meshes;
	return *this;
}

Mesh	MeshObject::processMesh(aiMesh *mesh, const aiScene* scene)
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
			//uv.x = mesh->mTextureCoords[0][i].x;
			//uv.y = mesh->mTextureCoords[0][i].y;
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
	processNode(scene->mRootNode, scene);
}

void	MeshObject::draw()
{
	for (size_t i = 0; i < meshes.size(); i++)
	{
		meshes[i].draw();
	}
}