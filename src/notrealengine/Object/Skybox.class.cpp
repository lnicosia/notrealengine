#include "Object/Skybox.class.hpp"
#include "CheckFileType.hpp"
#include "GLContext.class.hpp"
#include "pngparser/Png.class.hpp"

#ifdef USING_EXTERNAL_LIBS
//	Image loading library
# ifdef __unix__
#  pragma GCC diagnostic push
#  pragma GCC diagnostic ignored "-Wconversion"
# endif
# include "../lib/stb_image.h"
# ifdef __unix__
#  pragma GCC diagnostic pop
# endif
#endif

namespace notrealengine
{
	Skybox::Skybox(const std::string& path, const std::string paths[6]):
		Asset(path), VAO(0), VBO(0), glId(0), shader(GLContext::getShader("skybox"))
	{
		std::cout << "Loading skybox '" << path << "':" << std::endl;
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
			std::cout << "Loading skybox texture '" << paths[i];
			if (!IsReg(paths[i]))
			{
				std::cerr << "nre: Invalid file" << std::endl;
				this->loaded = false;
				return;
			}
			int	nChannels;

#ifdef USING_EXTERNAL_LIBS

			std::cout  << "' with stbi..." << std::endl;

			mft::vec2i size;
			stbi_set_flip_vertically_on_load(false);
			unsigned char* img = stbi_load(paths[i].c_str(), &size.x, &size.y, &nChannels, 0);
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

			GLCallThrow(glTexImage2D, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
				0, GL_RGB, size.x, size.y, 0, format, GL_UNSIGNED_BYTE, img);

			stbi_image_free(img);

#else

			//	Put custom PNG parser here

			std::cout << "' with custom parser..." << std::endl;

			std::filebuf fb;
			if (!fb.open(paths[i], std::ios_base::in | std::ios_base::binary))
			{
				std::cerr << std::endl << "nre:: Unable to open file \"" << path << "\"" << std::endl;
				this->loaded = false;
				return;
			}

			std::istream is(&fb);

			try
			{
				Png png(is);

				mft::vec2i size = png.getSize();
				GLCallThrow(glTexImage2D, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
					0, GL_RGBA, size.x, size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, png.getPixels().data());
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

			GLCallThrow(glTexParameteri, GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			GLCallThrow(glTexParameteri, GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			GLCallThrow(glTexParameteri, GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			GLCallThrow(glTexParameteri, GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			GLCallThrow(glTexParameteri, GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

		}
	}

	Skybox::~Skybox()
	{
		GLCallThrow(glDeleteTextures, 1, &this->glId);
		GLCallThrow(glDeleteVertexArrays, 1, &this->VAO);
		GLCallThrow(glDeleteVertexArrays, 1, &this->VBO);
	}

	Skybox::Skybox(Skybox&& ref) noexcept
		: Asset(std::move(ref)), VAO(std::exchange(ref.VAO, 0)), VBO(std::exchange(ref.VBO, 0)),
		glId(std::exchange(ref.glId, 0)), shader(std::exchange(ref.shader, nullptr))
	{
	}

	Skybox& Skybox::operator=(Skybox&& ref)
	{
		Asset::operator=(std::move(ref));
		VAO = std::exchange(ref.VAO, 0);
		VBO= std::exchange(ref.VBO, 0);
		glId= std::exchange(ref.glId, 0);
		shader =std::exchange(ref.shader, nullptr);
		return *this;
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
		if (this->shader == nullptr)
			return;
		glDepthFunc(GL_LEQUAL);
		GLCallThrow(glUseProgram, shader->programID);
		bindInt(shader->programID, "cubemap", 0);

		GLCallThrow(glBindVertexArray, this->VAO);
		GLCallThrow(glBindTexture, GL_TEXTURE_CUBE_MAP, this->glId);
		GLCallThrow(glDrawArrays, GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
		glDepthFunc(GL_LESS);
	}
}
