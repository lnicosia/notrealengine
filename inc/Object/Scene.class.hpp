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
			render();
		void
			renderBones();

		void
			addMesh(const std::shared_ptr<Mesh>& mesh);

		void
			addObject(const std::shared_ptr<GLObject>& object);

		void
			addLight(std::shared_ptr<Light>& light);

		void
			bindLights(unsigned int shader);

		void
			bindCamera();

		//	Accessors

		const mft::mat4&
			getProjectionMatrix() const;

		const Camera&
			getCamera() const;

		//	Setters

		/**	Set a new projection matrix and update all shaders uniform accordingly
		*/
		void
			setProjectionMatrix(const mft::mat4& projection);

		/**	Move the main camera forward into the scene
		*/
		void
			forward(uint32_t time);
		/**	Move the main camera backward into the scene
		*/
		void
			backward(uint32_t time);
		/**	Move the main camera left into the scene
		*/
		void
			left(uint32_t time);
		/**	Move the main camera right into the scene
		*/
		void
			right(uint32_t time);
		void
			lookDown(uint32_t time);
		void
			lookUp(uint32_t time);
		void
			lookLeft(uint32_t time);
		void
			lookRight(uint32_t time);
		void
			setCameraSpeed(float speed);

	private:
		std::string	name;
		
		std::vector<std::shared_ptr<GLObject>>	objects;
		std::vector<std::shared_ptr<Light>>		lights;
		/**	Keep track of all the shaders used in the scene
		*/
		std::vector<unsigned int>	shaders;

		mft::mat4	projection;
		mft::mat4	view;

		Camera		camera;
		uint32_t	currentTime;
		uint32_t	deltaTime;

		//std::map<unsigned int, std::vector<std::shared_ptr<Mesh>>>	shaders;
	};

}

#endif // !_SCENE_CLASS_H_
