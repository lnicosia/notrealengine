#include "Object/Light.class.hpp"
#include "Object/AssetManager.class.hpp"
#include "GLContext.class.hpp"

namespace notrealengine
{
	Light::Light(LightType type)
		: Asset(), transform(),
		VBO(0), VAO(0), texture(0),
		shader(GLContext::getShader("2dProjected")->programID), dirty(false)
	{
		name = "Light" + std::to_string(count);
		count++;

		//	2D Image setup
		float	vertices[] =
		{
			0.0f, 1.0f, 0.0f,  0.0f, 1.0f,
			1.0f, 0.0f, 0.0f,  1.0f, 0.0f,
			0.0f, 0.0f, 0.0f,  0.0f, 0.0f,

			0.0f, 1.0f, 0.0f,  0.0f, 1.0f,
			1.0f, 1.0f, 0.0f,  1.0f, 1.0f,
			1.0f, 0.0f, 0.0f,  1.0f, 0.0f
		};

		GLCallThrow(glGenBuffers, 1, &VBO);
		GLCallThrow(glGenVertexArrays, 1, &VAO);

		GLCallThrow(glBindBuffer, GL_ARRAY_BUFFER, VBO);
		GLCallThrow(glBufferData, GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		GLCallThrow(glBindVertexArray, VAO);

		GLCallThrow(glEnableVertexAttribArray, 0);
		GLCallThrow(glVertexAttribPointer, 0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);

		GLCallThrow(glEnableVertexAttribArray, 1);
		GLCallThrow(glVertexAttribPointer, 1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

		GLCallThrow(glBindBuffer, GL_ARRAY_BUFFER, 0);
		GLCallThrow(glBindVertexArray, 0);

		texture = AssetManager::getInstance().loadAsset<Texture>("resources/images/bulb-light.png", "diffuse")->getGLId();
	}

	Light::~Light()
	{
		GLCallThrow(glDeleteBuffers, 1, &VBO);
		GLCallThrow(glDeleteVertexArrays, 1, &VBO);
	}

	void	Light::move(mft::vec3 translation)
	{
		this->transform.move(translation);
		this->dirty = true;
	}

	void	Light::setDirty(bool state)
	{
		this->dirty = state;
	}

	const Transform& Light::getTransform() const
	{
		return transform;
	}

	const bool	Light::isDirty() const
	{
		return dirty;
	}

	const std::string Light::getAssetType() const
	{
		return std::string("Light");
	}

	void	Light::draw() const
	{
		GLCallThrow(glUseProgram, shader);
		bindMatrix(shader, "model", transform.getMatrix());

		GLCallThrow(glActiveTexture, GL_TEXTURE0);
		GLCallThrow(glBindTexture, GL_TEXTURE_2D, texture);
		bindInt(shader, "image", 0);

		GLCallThrow(glBindVertexArray, VAO);
		GLCallThrow(glDrawArrays, GL_TRIANGLES, 0, 6);
		GLCallThrow(glBindVertexArray, 0);

	}

	unsigned int	Light::count = 0;
}
