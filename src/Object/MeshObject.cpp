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

void	MeshObject::loadObject(std::string path)
{

}