#include "Object/Skybox.class.hpp"
#include "CheckFileType.hpp"
#include "GLContext.class.hpp"

//	Image loading library
# ifdef __unix__
#  pragma GCC diagnostic push
#  pragma GCC diagnostic ignored "-Wconversion"
# endif
# include "../lib/stb_image.h"
# ifdef __unix__
#  pragma GCC diagnostic pop
# endif

namespace notrealengine
{
	Skybox::Skybox(const std::string& path, const std::string paths[6]):
		Asset(path), VAO(0), VBO(0), glId(0), shader(GLContext::getShader("skybox"))
	{
		std::cout << "Loading skybox " << path << "..." << std::endl;
		float skybox_vertices[] = {
			-1.0f,  1.0f, -1.0f,
			-1.0f, -1.0f, -1.0f,
			1.0f, -1.0f, -1.0f,
			1.0f, -1.0f, -1.0f,
			1.0f,  1.0f, -1.0f,
			-1.0f,  1.0f, -1.0f,

			-1.0f, -1.0f,  1.0f,
			-1.0f, -1.0f, -1.0f,
			-1.0f,  1.0f, -1.0f,
			-1.0f,  1.0f, -1.0f,
			-1.0f,  1.0f,  1.0f,
			-1.0f, -1.0f,  1.0f,

			1.0f, -1.0f, -1.0f,
			1.0f, -1.0f,  1.0f,
			1.0f,  1.0f,  1.0f,
			1.0f,  1.0f,  1.0f,
			1.0f,  1.0f, -1.0f,
			1.0f, -1.0f, -1.0f,

			-1.0f, -1.0f,  1.0f,
			-1.0f,  1.0f,  1.0f,
			1.0f,  1.0f,  1.0f,
			1.0f,  1.0f,  1.0f,
			1.0f, -1.0f,  1.0f,
			-1.0f, -1.0f,  1.0f,

			-1.0f,  1.0f, -1.0f,
			1.0f,  1.0f, -1.0f,
			1.0f,  1.0f,  1.0f,
			1.0f,  1.0f,  1.0f,
			-1.0f,  1.0f,  1.0f,
			-1.0f,  1.0f, -1.0f,

			-1.0f, -1.0f, -1.0f,
			-1.0f, -1.0f,  1.0f,
			1.0f, -1.0f, -1.0f,
			1.0f, -1.0f, -1.0f,
			-1.0f, -1.0f,  1.0f,
			1.0f, -1.0f,  1.0f
		};

		GLCallThrow(glGenVertexArrays, 1, &this->VAO);
		GLCallThrow(glGenBuffers, 1, &this->VBO);

		GLCallThrow(glBindVertexArray, this->VAO);
		GLCallThrow(glBindBuffer, GL_ARRAY_BUFFER, this->VBO);
		GLCallThrow(glBufferData, GL_ARRAY_BUFFER, sizeof(skybox_vertices), skybox_vertices, GL_STATIC_DRAW);

		GLCallThrow(glVertexAttribPointer, 0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		GLCallThrow(glEnableVertexAttribArray, 0);

		GLCallThrow(glGenTextures, 1, &this->glId);
		GLCallThrow(glBindTexture, GL_TEXTURE_CUBE_MAP, this->glId);
		for (int i = 0; i < 6; i++)
		{
			if (!IsReg(paths[i]))
			{
				std::cerr << "nre: Invalid file" << std::endl;
				return;
			}
			int	nChannels;

#ifdef USING_EXTERNAL_LIBS

			stbi_set_flip_vertically_on_load(true);
			unsigned char* img = stbi_load(path.c_str(), &size.x, &size.y, &nChannels, 0);
			if (!img)
			{
				std::cerr << "Failed to load texture '" + path << " '" << std::endl;
				std::cerr << stbi_failure_reason() << std::endl;
				stbi_image_free(img);
				return;
			}
#else

			//	Put custom PNG parser here

			stbi_set_flip_vertically_on_load(false);
			mft::vec2i size;
			unsigned char* img = stbi_load(paths[i].c_str(), &size.x, &size.y, &nChannels, 0);
			if (!img)
			{
				std::cerr << "Failed to load texture '" + paths[i] << " '" << std::endl;
				std::cerr << stbi_failure_reason() << std::endl;
				stbi_image_free(img);
				return;
			}
#endif

			GLenum	format;
			if (nChannels == 1)
				format = GL_RED;
			else if (nChannels == 3)
				format = GL_RGB;
			else if (nChannels == 4)
				format = GL_RGBA;

			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
				0, GL_RGB, size.x, size.y, 0, format, GL_UNSIGNED_BYTE, img);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
			stbi_image_free(img);
		}
	}

	Skybox::~Skybox()
	{
		GLCallThrow(glDeleteTextures, 1, &this->glId);
		GLCallThrow(glDeleteVertexArrays, 1, &this->VAO);
		GLCallThrow(glDeleteVertexArrays, 1, &this->VBO);
	}

	//	Accessors

	const unsigned int& Skybox::getGLId() const
	{
		return glId;
	}

	const GLShaderProgram* Skybox::getShader() const
	{
		return shader;
	}

	const unsigned int	Skybox::getShaderID() const
	{
		if (shader != nullptr)
			return shader->programID;
		return 0;
	}

	//	Setters

	void	Skybox::setShader(GLShaderProgram* shader)
	{
		this->shader = shader;
	}

	//	Asset class

	const std::string	Skybox::getAssetType() const
	{
		return "Skybox";
	}

	//	Draw

	void	Skybox::draw() const
	{
		glDepthFunc(GL_LEQUAL);
		if (this->shader == nullptr)
			return;
		GLCallThrow(glUseProgram, shader->programID);
		bindInt(shader->programID, "cubemap", 0);
			
		GLCallThrow(glBindVertexArray, this->VAO);
		GLCallThrow(glBindTexture, GL_TEXTURE_CUBE_MAP, this->glId);
		GLCallThrow(glDrawArrays, GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
		glDepthFunc(GL_LESS);
	}
}