#ifndef _SCENE_CLASS_H_
#define _SCENE_CLASS_H_

#include "Object/GLObject.class.hpp"
#include "Camera.class.hpp"

namespace notrealengine
{
	class Scene
	{
	public:
		Scene();
		~Scene();

		void
			render() const;

	private:
		Camera	camera;
		std::vector<std::shared_ptr<GLObject>>	objects;
	};

}

#endif // !_SCENE_CLASS_H_
