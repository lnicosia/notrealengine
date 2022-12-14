#ifndef _SCENE_CLASS_H_
#define _SCENE_CLASS_H_

#include "Object/GLObject.class.hpp"
#include "Object/Light.class.hpp"
#include "Object/Skybox.class.hpp"
#include "Camera.class.hpp"

#define MAX_LIGHTS 4

enum LightingMode
{
	Lit,
	Unlit,
};
enum DrawMode
{
	Fill,
	Wireframe
};

namespace notrealengine
{
	class Scene
	{
	public:

		Scene();
		Scene(const std::string& name);
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
			setSkybox(std::shared_ptr<Skybox>);

		void
			bindMatrices(unsigned int shader) const;

		void
			bindLights(unsigned int shader) const;

		void
			bindCamera() const;

		//	Accessors

		const mft::mat4&
			getProjectionMatrix() const;

		const Camera&
			getCamera() const;

		const LightingMode
			getLightingMode() const;

		const DrawMode
			getDrawMode() const;

		const std::shared_ptr<Skybox>
			getSkybox() const;

		//	Setters

		/**	Set a new projection matrix and update all shaders uniform accordingly
		*/
		void
			setProjectionMatrix(const mft::mat4& projection);

		/**	Camera functions
		*/

		//	Movement functions

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

		/**	Move the main camera forward into the scene
		*/
		void
			forward(float value);
		/**	Move the main camera backward into the scene
		*/
		void
			backward(float value);
		/**	Move the main camera left into the scene
		*/
		void
			left(float value);
		/**	Move the main camera right into the scene
		*/
		void
			right(float value);

		//	View functions

		const float
			getYaw( void ) const;
		const float
			getPitch( void ) const;
		const float
			getCameraSpeed( void ) const;

		void
			setYaw(float yaw);
		void
			setPitch(float pitch);
		void
			setCameraPos(const mft::vec3& pos);
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
		void
			setLightingMode(LightingMode mode);
		void
			setDrawMode(DrawMode mode);

		//	Free function

		/**	Release all shared ptrs so they can be freed before OpenGL.
		**	Needs to be called manually if the scene is a global object
		*/
		void
			clear( void );

		bool	drawGrid;
		bool	drawSkybox;

	private:
		std::string	name;

		LightingMode	lightingMode;
		DrawMode			drawMode;

		std::vector<std::shared_ptr<GLObject>>	objects;
		std::vector<std::shared_ptr<Light>>		lights;
		std::shared_ptr<Skybox>	skybox;
		/**	Keep track of all the shaders used in the scene
		*/
		std::vector<unsigned int>	shaders;

		mft::mat4	projection;
		mft::mat4	view;

		/**	Scene's camera. Private so we can update shaders when changing the camera
		*/
		Camera		camera;
		uint32_t	currentTime;
		uint32_t	deltaTime;

		//std::map<unsigned int, std::vector<std::shared_ptr<Mesh>>>	shaders;
	};

}

#endif // !_SCENE_CLASS_H_
