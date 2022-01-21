#include "Object/Scene.class.hpp"
#include "GLContext.class.hpp"
#include "GL.hpp"


namespace notrealengine
{
	Scene::Scene(): name(), camera(mft::vec3(0.0f, 0.0f, -5.0f)),
		objects(), lights(), shaders(),
		projection(mft::mat4::perspective(mft::radians(45.0f), 16.0f / 9.0f, 0.1f, 10000.0f)),
		view()
	{
		shaders.push_back(GLContext::getShader("2dProjected")->programID);
		mft::mat4	ortho = mft::mat4::ortho(0.0f, 1600.0f, 0.0f, 900.0f);

		GLCallThrow(glUseProgram, GLContext::getShader("text")->programID);
		GLint location = GLCallThrow(glGetUniformLocation, GLContext::getShader("text")->programID, "projection");
		GLCallThrow(glUniformMatrix4fv, location, 1, GL_TRUE, static_cast<float*>(ortho));
		//	Binding 2d shader manually
		GLCallThrow(glUseProgram, GLContext::getShader("2dProjected")->programID);
		location = GLCallThrow(glGetUniformLocation, GLContext::getShader("2dProjected")->programID, "projection");
		GLCallThrow(glUniformMatrix4fv, location, 1, GL_TRUE, static_cast<float*>(this->projection));
		location = GLCallThrow(glGetUniformLocation, GLContext::getShader("2dProjected")->programID, "view");
		GLCallThrow(glUniformMatrix4fv, location, 1, GL_TRUE, static_cast<const float*>(this->camera.getViewMatrix()));
		//	Binding color shader manually (default shader)
		GLCallThrow(glUseProgram, GLContext::getShader("color")->programID);
		location = GLCallThrow(glGetUniformLocation, GLContext::getShader("color")->programID, "projection");
		GLCallThrow(glUniformMatrix4fv, location, 1, GL_TRUE, static_cast<float*>(this->projection));
		location = GLCallThrow(glGetUniformLocation, GLContext::getShader("color")->programID, "view");
		GLCallThrow(glUniformMatrix4fv, location, 1, GL_TRUE, static_cast<const float*>(this->camera.getViewMatrix()));
	}

	Scene::~Scene()
	{
	}

	//	Accessors

	const mft::mat4& Scene::getProjectionMatrix() const
	{
		return projection;
	}

	const Camera& Scene::getCamera() const
	{
		return camera;
	}

	//	Setters

	void	Scene::setProjectionMatrix(const mft::mat4& projection)
	{
		this->projection = projection;
		for (const auto& shader : shaders)
		{
			GLCallThrow(glUseProgram, shader);
			GLint location;
			location = GLCallThrow(glGetUniformLocation, shader, "projection");
			GLCallThrow(glUniformMatrix4fv, location, 1, GL_TRUE, static_cast<float*>(this->projection));
		}
		//	Bind color shader manually (default shader)
		unsigned int shader = GLContext::getShader("color")->programID;
		GLCallThrow(glUseProgram, shader);
		GLint location;
		location = GLCallThrow(glGetUniformLocation, shader, "projection");
		GLCallThrow(glUniformMatrix4fv, location, 1, GL_TRUE, static_cast<float*>(this->projection));
	}

	void	Scene::forward(uint32_t time)
	{
		this->camera.forward(time);
		bindCamera();
	}

	void	Scene::backward(uint32_t time)
	{
		this->camera.backward(time);
		bindCamera();
	}

	void	Scene::left(uint32_t time)
	{
		this->camera.left (time);
		bindCamera();
	}

	void	Scene::right(uint32_t time)
	{
		this->camera.right(time);
		bindCamera();
	}

	void	Scene::lookDown(uint32_t time)
	{
		this->camera.pitch += 1;
		this->camera.update();
		bindCamera();
	}

	void	Scene::lookUp(uint32_t time)
	{
		this->camera.pitch -= 1;
		this->camera.update();
		bindCamera();
	}

	void	Scene::lookLeft(uint32_t time)
	{
		this->camera.yaw += 1;
		this->camera.update();
		bindCamera();
	}

	void	Scene::lookRight(uint32_t time)
	{
		this->camera.yaw -= 1;
		this->camera.update();
		bindCamera();
	}

	void	Scene::setCameraSpeed(float speed)
	{
		this->camera.speed = speed;
	}

	void	Scene::bindCamera()
	{
		for (const auto& shader: shaders)
		{
			GLCallThrow(glUseProgram, shader);
			GLint location;
			location = GLCallThrow(glGetUniformLocation, shader, "view");
			GLCallThrow(glUniformMatrix4fv, location, 1, GL_TRUE, static_cast<const float*>(this->camera.getViewMatrix()));
		}
		//	Bind color shader manually (default shader)
		unsigned int shader = GLContext::getShader("color")->programID;
		GLCallThrow(glUseProgram, shader);
		GLint location;
		location = GLCallThrow(glGetUniformLocation, shader, "view");
		GLCallThrow(glUniformMatrix4fv, location, 1, GL_TRUE, static_cast<const float*>(this->camera.getViewMatrix()));
	}

	void	Scene::bindLights(unsigned int shader)
	{
		GLCallThrow(glUseProgram, shader);
		GLint	location;
		for (int i = 0; i < lights.size(); i++)
		{
			std::string	name = "pointLights[" + std::to_string(i) + "].constant";
			location = GLCallThrow(glGetUniformLocation, shader, name.c_str());
			GLCallThrow(glUniform1f, location, 1.0f);
			name = "pointLights[" + std::to_string(i) + "].linear";
			location = GLCallThrow(glGetUniformLocation, shader, name.c_str());
			GLCallThrow(glUniform1f, location, 0.09f);
			name = "pointLights[" + std::to_string(i) + "].quadratic";
			location = GLCallThrow(glGetUniformLocation, shader, name.c_str());
			GLCallThrow(glUniform1f, location, 0.032f);
			name = "pointLights[" + std::to_string(i) + "].pos";
			location = GLCallThrow(glGetUniformLocation, shader, name.c_str());
			GLCallThrow(glUniform3fv, location, 1, &lights[i]->getTransform().getPos()[0]);
			name = "pointLights[" + std::to_string(i) + "].ambient";
			location = GLCallThrow(glGetUniformLocation, shader, name.c_str());
			mft::vec3 ambient(0.0f, 0.0f, 0.0f);
			GLCallThrow(glUniform3fv, location, 1, &ambient[0]);
			name = "pointLights[" + std::to_string(i) + "].diffuse";
			location = GLCallThrow(glGetUniformLocation, shader, name.c_str());
			mft::vec3 diffuse(0.5f, 0.5f, 0.5f);
			GLCallThrow(glUniform3fv, location, 1, &diffuse[0]);
			name = "pointLights[" + std::to_string(i) + "].specular";
			location = GLCallThrow(glGetUniformLocation, shader, name.c_str());
			mft::vec3 specular(0.0f, 0.0f, 0.0f);
			GLCallThrow(glUniform3fv, location, 1, &specular[0]);
		}
		GLCallThrow(glUseProgram, 0);
	}

	void	Scene::addMesh(const std::shared_ptr<Mesh>& mesh)
	{
		//shaders[mesh->getShader()].push_back(mesh);
		unsigned int	shader = mesh->getShader();
		//	Add this mesh's shader to the scene
		if (std::find(shaders.begin(), shaders.end(), shader) == shaders.end())
		{
			shaders.push_back(shader);
			GLCallThrow(glUseProgram, shader);
			GLint location = GLCallThrow(glGetUniformLocation, shader, "view");
			GLCallThrow(glUniformMatrix4fv, location, 1, GL_TRUE, static_cast<const float*>(this->camera.getViewMatrix()));
			location = GLCallThrow(glGetUniformLocation, shader, "projection");
			GLCallThrow(glUniformMatrix4fv, location, 1, GL_TRUE, static_cast<const float*>(this->projection));
			bindLights(shader);
		}
		std::vector<std::shared_ptr<Mesh>>	children = mesh->getChildren();
		for (const auto& child: children)
		{
			addMesh(child);
		}
	}

	void	Scene::addObject(const std::shared_ptr<GLObject>& object)
	{
		std::vector<std::shared_ptr<Mesh>>	meshes = object->getMeshes();
		for (const auto& mesh: meshes)
		{
			addMesh(mesh);
		}
		objects.push_back(object);
	}

	void	Scene::addLight(std::shared_ptr<Light>& light)
	{
		if (lights.size() >= MAX_LIGHTS)
		{
			std::cerr << "Maximum number of lights reach in the scene" << std::endl;
			return;
		}
		lights.push_back(light);
		for (const auto& shader : shaders)
		{
			bindLights(shader);
		}
	}

	void	Scene::render()
	{
		/*for (const auto& pair: shaders)
		{
			std::vector<std::shared_ptr<Mesh>> meshes = pair.second;
			for (const auto& mesh: meshes)
			{
				mesh->draw(mft::mat4());
			}
		}*/
		for (const auto& object: objects)
		{
			object->draw();
		}
		for (const auto& light: lights)
		{
			if (light->isDirty())
			{
				bindLights(GLContext::getShader("default")->programID);
			}
			//light->draw();
		}
	}

	void	Scene::renderBones()
	{
		for (const auto& object : objects)
		{
			object->drawBones();
		}
	}
}
