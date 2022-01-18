#include "Object/Texture.class.hpp"
#include "GL.hpp"

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

#include <iostream>

namespace notrealengine
{
	Texture::Texture(const std::string& path, const std::string& type)
		: Asset({path}), type(type), glId(0)
	{

		int	w, h, nChannels;
		std::cout << "Loading texture '" << path << "'..." << std::endl;
		stbi_set_flip_vertically_on_load(true);
		unsigned char* img = stbi_load(path.c_str(), &w, &h, &nChannels, 0);
		if (!img)
		{
			std::cerr << "Failed to load texture '" + path << " '" << std::endl;
			std::cerr << stbi_failure_reason() << std::endl;
			stbi_image_free(img);
			return;
		}
		GLenum	format;
		if (nChannels == 1)
			format = GL_RED;
		else if (nChannels == 3)
			format = GL_RGB;
		else if (nChannels == 4)
			format = GL_RGBA;

		GLCallThrow(glGenTextures, 1, &glId);
		GLCallThrow(glBindTexture, GL_TEXTURE_2D, glId);
		GLCallThrow(glTexImage2D, GL_TEXTURE_2D, 0, (GLint)format, w, h, 0, format, GL_UNSIGNED_BYTE, img);
		GLCallThrow(glGenerateMipmap, GL_TEXTURE_2D);
		GLCallThrow(glTexParameteri, GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		GLCallThrow(glTexParameteri, GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		GLCallThrow(glTexParameteri, GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		GLCallThrow(glTexParameteri, GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		stbi_image_free(img);
	}

	Texture::Texture(const std::string& path, unsigned char *data, unsigned int width, std::string const& type)
		: Asset({path}), type(type), glId(0)
	{

		int	w, h, nChannels;
		std::cout << "Loading embedded texture in '" << path << "'..." << std::endl;
		unsigned char* img = stbi_load_from_memory(data, width, &w, &h, &nChannels, 0);
		if (!img)
		{
			std::cerr << "Failed to load texture '" + path << " '" << std::endl;
			std::cerr << stbi_failure_reason() << std::endl;
			stbi_image_free(img);
			return;
		}
		GLenum	format;
		if (nChannels == 1)
			format = GL_RED;
		else if (nChannels == 3)
			format = GL_RGB;
		else if (nChannels == 4)
			format = GL_RGBA;

		GLCallThrow(glGenTextures, 1, &glId);
		GLCallThrow(glBindTexture, GL_TEXTURE_2D, glId);
		GLCallThrow(glTexImage2D, GL_TEXTURE_2D, 0, (GLint)format, w, h, 0, format, GL_UNSIGNED_BYTE, img);
		GLCallThrow(glGenerateMipmap, GL_TEXTURE_2D);
		GLCallThrow(glTexParameteri, GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		GLCallThrow(glTexParameteri, GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		GLCallThrow(glTexParameteri, GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		GLCallThrow(glTexParameteri, GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		stbi_image_free(img);
	}

	Texture::Texture(Texture && ref) noexcept
		: Asset(std::move(ref)), glId(std::exchange(ref.glId, 0)), type(std::move(ref.type))
	{

	}

	Texture& Texture::operator=(Texture&& text) noexcept
	{
		Asset::operator=(std::move(text));
		this->glId = std::exchange(text.glId, 0);
		this->type = std::move(text.type);

		return *this;
	}

	Texture::~Texture()
	{
		glDeleteBuffers(1, &glId);
	}

	//	Accessors

	const unsigned int& Texture::getGLId() const
	{
		return glId;
	}

	const std::string& Texture::getType() const
	{
		return type;
	}

	const std::string	Texture::getAssetType() const
	{
		return std::string("Texture");
	}

	//	Setters

	void	Texture::setType(const std::string& type)
	{
		this->type = type;
	}
}
