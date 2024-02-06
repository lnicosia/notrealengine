#include "Object/Texture.class.hpp"
#include "GL.hpp"
#include "GLContext.class.hpp"
#include "UsingExternalLibs.hpp"
#include "CheckFileType.hpp"
#include "pngparser/Png.class.hpp"


//	OpenGL includes
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
# include <windows.h>
# include <glad/glad.h>
#else
# define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>
#endif

#ifdef USING_EXTERNAL_LIBS
//	Image loading library
# ifdef __unix__
#  pragma GCC diagnostic push
#  pragma GCC diagnostic ignored "-Wconversion"
# endif
# define STB_IMAGE_IMPLEMENTATION
# include "../lib/stb_image.h"
# ifdef __unix__
#  pragma GCC diagnostic pop
# endif
#endif

#include <iostream>
#include <fstream>

namespace notrealengine
{
	Texture::Texture(std::string const& path, std::string const& type)
		: Asset({ path }), type(type), glId(0), VAO(0), VBO(0), size(0, 0),
		shader(GLContext::getShader("2d"))
	{
		std::cout << "Loading texture '" << path;
		if (!IsReg(path))
		{
			std::cerr << std::endl << "nre: Invalid file" << std::endl;
			this->loaded = false;
			return;
		}

		//	2D Image setup
		float	vertices[] =
		{
			0.0f, 1.0f, 0.0f, 1.0f,
			1.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 0.0f,

			0.0f, 1.0f, 0.0f, 1.0f,
			1.0f, 1.0f, 1.0f, 1.0f,
			1.0f, 0.0f, 1.0f, 0.0f
		};

		GLCallThrow(glGenBuffers, 1, &this->VBO);
		GLCallThrow(glGenVertexArrays, 1, &this->VAO);

		GLCallThrow(glBindBuffer, GL_ARRAY_BUFFER, this->VBO);
		GLCallThrow(glBufferData, GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		GLCallThrow(glBindVertexArray, this->VAO);
		GLCallThrow(glEnableVertexAttribArray, 0);
		GLCallThrow(glVertexAttribPointer, 0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);

		GLCallThrow(glBindBuffer, GL_ARRAY_BUFFER, 0);
		GLCallThrow(glBindVertexArray, 0);

		//	Texture generation

		GLCallThrow(glGenTextures, 1, &this->glId);
		GLCallThrow(glBindTexture, GL_TEXTURE_2D, this->glId);

#ifdef USING_EXTERNAL_LIBS

		std::cout << "' with stbi..." << std::endl;

		int	nChannels;

		stbi_set_flip_vertically_on_load(true);
		unsigned char* img = stbi_load(path.c_str(), &size.x, &size.y, &nChannels, 0);
		if (!img)
		{
			std::cerr << std::endl << "Failed to load texture '" + path << " '" << std::endl;
			this->loaded = false;
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

		GLCallThrow(glTexImage2D, GL_TEXTURE_2D, 0, (GLint)format,
			this->size.x, this->size.y, 0, format, GL_UNSIGNED_BYTE, img);
		stbi_image_free(img);

#else

		std::cout << "' with custom parser..." << std::endl;

		std::filebuf fb;
		if (!fb.open(path, std::ios_base::in | std::ios_base::binary))
		{
			std::cerr << std::endl << "nre:: Unable to open file \"" << path << "\"" << std::endl;
			this->loaded = false;
			return;
		}

		std::istream is(&fb);

		try
		{
			Png png(is);

			this->size = png.getSize();
			const std::vector<unsigned char>& pixels = png.getPixels();
			std::vector<unsigned char> flipped(pixels.size(), 0);
			for (size_t y = 0; y < size.y; y++)
			{
				for (size_t x = 0; x < size.x * 4; x += 4)
				{
					flipped[x + y * size.x * 4 + 0] = pixels[x + (size.y - 1 - y) * size.x * 4 + 0];
					flipped[x + y * size.x * 4 + 1] = pixels[x + (size.y - 1 - y) * size.x * 4 + 1];
					flipped[x + y * size.x * 4 + 2] = pixels[x + (size.y - 1 - y) * size.x * 4 + 2];
					flipped[x + y * size.x * 4 + 3] = pixels[x + (size.y - 1 - y) * size.x * 4 + 3];
				}
			}
			GLCallThrow(glTexImage2D, GL_TEXTURE_2D, 0, (GLint)GL_RGBA,
				this->size.x, this->size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, flipped.data());

			fb.close();
		}
		catch (png_exception& e)
		{
			std::cerr << std::endl << e.what() << std::endl;
			this->loaded = false;
			return;
		}

		fb.close();

#endif

		GLCallThrow(glGenerateMipmap, GL_TEXTURE_2D);
		GLCallThrow(glTexParameteri, GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		GLCallThrow(glTexParameteri, GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		GLCallThrow(glTexParameteri, GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		GLCallThrow(glTexParameteri, GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	}

	Texture::Texture(const std::string& path, unsigned char *data, unsigned int width, std::string const& type)
		: Asset({ path }), type(type), glId(0), VAO(0), VBO(0), size(0, 0),
		shader(GLContext::getShader("2d"))
	{

		int	w, h, nChannels;
		std::cout << "Loading embedded texture in'" << path << "'..." << std::endl;
		if (!IsReg(path))
		{
			std::cerr << "nre: Invalid file" << std::endl;
			this->loaded = false;
			return;
		}

		//	Texture generation

		GLCallThrow(glGenTextures, 1, &this->glId);
		GLCallThrow(glBindTexture, GL_TEXTURE_2D, this->glId);

#ifdef USING_EXTERNAL_LIBS

		unsigned char* img = stbi_load_from_memory(data, width, &w, &h, &nChannels, 0);
		if (!img)
		{
			std::cerr << "Failed to load texture '" + path << " '" << std::endl;
			this->loaded = false;
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

		GLCallThrow(glTexImage2D, GL_TEXTURE_2D, 0, (GLint)format, w, h, 0, format, GL_UNSIGNED_BYTE, img);
		stbi_image_free(img);

#else


		//	Put custom PNG parser here
		return;

#endif

		GLCallThrow(glGenerateMipmap, GL_TEXTURE_2D);
		GLCallThrow(glTexParameteri, GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		GLCallThrow(glTexParameteri, GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		GLCallThrow(glTexParameteri, GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		GLCallThrow(glTexParameteri, GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	}

	Texture::Texture(Texture && ref) noexcept
		: Asset(std::move(ref)), glId(std::exchange(ref.glId, 0)), type(std::move(ref.type)),
			VAO(std::exchange(ref.VAO, 0)), VBO(std::exchange(ref.VBO, 0)),
			size(std::move(ref.size))
	{

	}

	Texture& Texture::operator=(Texture&& text) noexcept
	{
		Asset::operator=(std::move(text));
		this->glId = std::exchange(text.glId, 0);
		this->type = std::move(text.type);
		this->size = std::move(text.size);

		return *this;
	}

	Texture::~Texture()
	{
		GLCallThrow(glDeleteTextures, 1, &this->glId);
		GLCallThrow(glDeleteBuffers, 1, &this->VBO);
		GLCallThrow(glDeleteVertexArrays, 1, &this->VAO);
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

	const mft::vec2i& Texture::getSize() const
	{
		return size;
	}

	const GLShaderProgram* Texture::getShader() const
	{
		return this->shader;
	}

	const unsigned int Texture::getShaderID() const
	{
		if (this->shader != nullptr)
			return this->shader->programID;
		return 0;
	}

	//	Setters

	void	Texture::setType(const std::string& type)
	{
		this->type = type;
	}

	void	Texture::setShader(GLShaderProgram* shader)
	{
		this->shader = shader;
	}

	void	Texture::draw(mft::vec2i pos, mft::vec2i size, float rotation, mft::vec4 color,
		GLShaderProgram* shader) const
	{
		if (shader == nullptr)
		{
			if (this->shader == nullptr)
				return;
			shader = this->shader;
		}
		mft::mat4	model;

		model *= mft::mat4::translate(mft::vec3(pos.x, pos.y, 0.0f));
		model *= mft::mat4::translate(mft::vec3(0.5f * size.x, 0.5f * size.y, 0.0f));
		model *= mft::mat4::rotate(mft::radians(rotation), mft::vec3(0.0f, 0.0f, 1.0f));
		model *= mft::mat4::translate(mft::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f));
		model *= mft::mat4::scale(mft::vec3(size.x, size.y, 1.0f));

		bindVector(shader->programID, "color", color);
		bindMatrix(shader->programID, "model", model);
		GLCallThrow(glActiveTexture, GL_TEXTURE0);
		GLCallThrow(glBindTexture, GL_TEXTURE_2D, glId);

		GLCallThrow(glBindVertexArray, VAO);
		GLCallThrow(glDrawArrays, GL_TRIANGLES, 0, 6);
		GLCallThrow(glBindVertexArray, 0);
	}
}
