#ifndef _SCENE_CLASS_H_
#define _SCENE_CLASS_H_

#include "Object/GLObject.class.hpp"
#include "Object/Light.class.hpp"
#include "Camera.class.hpp"

#define MAX_LIGHTS 4

namespace notrealengine
{
	class Scene
	{
	public:
		Scene();
		~Scene();

		void
			render() const;

		void
			addMesh(const std::shared_ptr<Mesh>& mesh);

		void
			addObject(const std::shared_ptr<GLObject>& object);

		void
			addLight(std::shared_ptr<Light>& light);

	private:
		std::string	name;
		Camera	camera;
		std::vector<std::shared_ptr<GLObject>>	objects;
		std::vector<std::shared_ptr<Light>>			lights;

		std::map<unsigned int, std::vector<std::shared_ptr<Mesh>>>	shaders;
	};

}

#endif // !_SCENE_CLASS_H_
