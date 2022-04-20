#include "Object/Scene.class.hpp"
#include "GLContext.class.hpp"
#include "GL.hpp"


namespace notrealengine
{
	Scene::Scene()
	{
	}

	Scene::Scene(const std::string& name): name(name), camera(mft::vec3(0.0f, 3.0f, -5.0f)),
		objects(), lights(), shaders(),
		projection(mft::mat4::perspective(mft::radians(45.0f), 16.0f / 9.0f, 0.1f, 10000.0f)),
		view(), lightingMode(LightingMode::Unlit), drawMode(DrawMode::Fill),
		drawGrid(false)
	{
		camera.pitch += 15;
		camera.update();

		shaders.push_back(GLContext::getShader("2dProjected")->programID);
		mft::mat4	ortho = mft::mat4::ortho(0.0f, 1600.0f, 0.0f, 900.0f);

		bindMatrix(GLContext::getShader("text")->programID, "projection", ortho);
		bindMatrix(GLContext::getShader("2d")->programID, "projection", ortho);
		//	Binding matrices and light manually for global shaders
		bindMatrices(GLContext::getShader("2dProjected")->programID);

		bindMatrices(GLContext::getShader("color")->programID);
		bindLights(GLContext::getShader("color")->programID);

		bindMatrices(GLContext::getShader("colorUnlit")->programID);
		bindLights(GLContext::getShader("colorUnlit")->programID);

		bindMatrices(GLContext::getShader("bonesInfluence")->programID);
		bindLights(GLContext::getShader("bonesInfluence")->programID);
	}

	Scene::~Scene()
	{
	}

	void Scene::clear( void )
	{
		objects.clear();
		lights.clear();
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
			bindMatrix(shader, "projection", this->projection);
		}
		//	Bind color shader manually (default shader)
		bindMatrix(GLContext::getShader("color")->programID, "projection", this->projection);
		//	Bind color (no lighting) shader manually
		bindMatrix(GLContext::getShader("colorUnlit")->programID, "projection", this->projection);
	}

	/**	Camera functions
	*/

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

	const	float	Scene::getYaw( void ) const
	{
		return this->camera.yaw;
	}

	const	float	Scene::getPitch( void ) const
	{
		return this->camera.pitch;
	}

	const LightingMode	Scene::getLightingMode() const
	{
		return this->lightingMode;
	}

	const DrawMode	Scene::getDrawMode() const
	{
		return this->drawMode;
	}

	void	Scene::setYaw(float yaw)
	{
		this->camera.yaw = yaw;
		this->camera.update();
		bindCamera();
	}

	void	Scene::setPitch(float pitch)
	{
		this->camera.pitch = pitch;
		this->camera.update();
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

	void Scene::setLightingMode(LightingMode mode)
	{
		switch (mode)
		{
			case LightingMode::Lit:
				for (auto& obj: objects)
				{
					if (obj->getShader() == GLContext::getShader("unlit")->programID)
						obj->setShader(GLContext::getShader("default"));
					else if (obj->getShader() == GLContext::getShader("colorUnlit")->programID)
						obj->setShader(GLContext::getShader("color"));
					obj->bindBones();
					this->bindMatrices(GLContext::getShader("default")->programID);
					this->bindMatrices(GLContext::getShader("color")->programID);
				}
				bindLights(GLContext::getShader("default")->programID);
				break;
			case LightingMode::Unlit:
				for (auto& obj: objects)
				{
					if (obj->getShader() == GLContext::getShader("default")->programID)
						obj->setShader(GLContext::getShader("unlit"));
					else if (obj->getShader() == GLContext::getShader("color")->programID)
						obj->setShader(GLContext::getShader("colorUnlit"));
					obj->bindBones();
					this->bindMatrices(GLContext::getShader("unlit")->programID);
					this->bindMatrices(GLContext::getShader("colorUnlit")->programID);
				}
				bindLights(GLContext::getShader("default")->programID);
				break;
			default:
				break;
		}
		this->lightingMode = mode;
	}

	void Scene::setDrawMode(DrawMode mode)
	{
		this->drawMode = mode;
	}

	void	Scene::bindMatrices(unsigned int shader) const
	{
		bindMatrix(shader, "projection", this->projection);
		bindMatrix(shader, "view", this->camera.getViewMatrix());
		bindLights(shader);
	}

	void	Scene::bindCamera() const
	{
		for (const auto& shader: shaders)
		{
			bindMatrix(shader, "view", this->camera.getViewMatrix());
		}
		for (auto& obj: objects)
		{
			bindMatrix(obj->getShader(), "view", this->camera.getViewMatrix());
		}
		//	Manually bind global shaders
		bindMatrix(GLContext::getShader("color")->programID, "view", this->camera.getViewMatrix());
		bindMatrix(GLContext::getShader("colorUnlit")->programID, "view", this->camera.getViewMatrix());
		bindMatrix(GLContext::getShader("bonesInfluence")->programID, "view", this->camera.getViewMatrix());
	}

	void	Scene::bindLights(unsigned int shader) const
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
		if (this->drawMode == DrawMode::Wireframe || this->drawGrid == true)
			GLCallThrow(glPolygonMode, GL_FRONT_AND_BACK, GL_LINE);
		if (this->drawGrid == true)
		{
			GLContext::grid->draw();
		}
		if (this->drawMode != DrawMode::Wireframe)
			GLCallThrow(glPolygonMode, GL_FRONT_AND_BACK, GL_FILL);
		for (const auto& object: objects)
		{
			if (object->visible == true)
				object->draw();
		}
		for (const auto& light: lights)
		{
			if (light->isDirty())
			{
				bindLights(GLContext::getShader("default")->programID);
				bindLights(GLContext::getShader("color")->programID);
			}
			light->draw();
		}
		if (this->drawMode == DrawMode::Wireframe)
			GLCallThrow(glPolygonMode, GL_FRONT_AND_BACK, GL_FILL);
	}

	void	Scene::renderBones()
	{
		for (const auto& object : objects)
		{
			if (object->visible == true)
				object->drawBones();
		}
	}
}
