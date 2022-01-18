#include "Object/Scene.class.hpp"

namespace notrealengine
{
	Scene::Scene(): name(), camera(), objects(), lights(), shaders()
	{
	}

	Scene::~Scene()
	{
	}

	void Scene::addMesh(const std::shared_ptr<Mesh>& mesh)
	{
		shaders[mesh->getShader()].push_back(mesh);
		std::vector<std::shared_ptr<Mesh>>	children = mesh->getChildren();
		for (const auto& child: children)
		{
			addMesh(child);
		}
	}

	void Scene::addObject(const std::shared_ptr<GLObject>& object)
	{
		/*std::vector<std::shared_ptr<Mesh>>	meshes = object->getMeshes();
		for (const auto& mesh: meshes)
		{
			addMesh(mesh);
		}*/
		objects.push_back(object);
	}

	void Scene::addLight(std::shared_ptr<Light>& light)
	{
		if (lights.size() < MAX_LIGHTS)
			lights.push_back(light);
		else
			std::cerr << "Maximum number of lights reach in the scene" << std::endl;
	}

	void Scene::render() const
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
			light->draw();
		}
	}
}
