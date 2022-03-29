#include "SDL.h"
#include "mft/mft.hpp"
#include "notrealengine/notrealengine.hpp"
#include "Object/GLObject.class.hpp"
#include "Object/Animation.class.hpp"
#include "Object/TextureLoader.class.hpp"
#include "Object/AssetManager.class.hpp"
#include "Object/Light.class.hpp"
#include "Object/Scene.class.hpp"
#include "ColladaParser/ColladaSceneBuilder.class.hpp"
#include "LXML/LXML.class.hpp"
#include "GLShaderProgram.class.hpp"
#include "TextRendering/GLFont.class.hpp"
#include "Camera.class.hpp"
#include "Inputs/SDLEvents.class.hpp"
#include "Tests/RunTests.hpp"
#include <vector>

enum RenderingMode
{
	Object,
	Bob,
	Bones
};

using namespace notrealengine;

std::shared_ptr<GLObject>	InitBobby(void);
std::shared_ptr<Animation>	InitBobbyWalking(void);

int		main(int ac, char** av)
{
	if (ac < 2)
		return 1;

	bool shouldRunTests = true;
	for (int i = 1; i < ac; i++)
	{
		if (strcmp(av[i], "-noTests") == 0)
			shouldRunTests = false;
	}
	if (shouldRunTests == true)
		runTests("test/testList.txt");
	SDLWindow window("Not real engine", std::pair<int, int>(1600, 900));
	GLContext_SDL	context(window.getContext(), window.getWindow());
	context.makeCurrent();
	SDL_Event	e;
	int	running = 1;
	int	mode = Bob;
	int selectedBone = 0;

	std::shared_ptr<GLObject>	obj = AssetManager::getInstance().loadAsset<GLObject>(av[1]);
	std::shared_ptr<Animation>	anim;
	if (ac == 3 && av[2][0] != '-')
		anim = AssetManager::getInstance().loadAsset<Animation>(av[2], 0);
	else if (ac >= 2)
	 	anim = AssetManager::getInstance().loadAsset<Animation>(av[1], 0);
	std::shared_ptr<Animation> bobbyWalking = InitBobbyWalking();
	std::shared_ptr<Light>	light1(new Light(LightType::Directional));
	light1->move(mft::vec3(0.0f, 4.0f, -5.0f));

	obj->transform.rotate(mft::quat::rotation(mft::vec3(0.0f, 1.0f, 0.0f), mft::radians(180.0f)));
	//obj->transform.move(mft::vec3(0.0f, -5.0f, 10.0f));
	//obj->setShader(context.getShader("color"));

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	//glEnable(GL_CULL_FACE);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	std::shared_ptr<GLObject>	bobby = InitBobby();


	//upperLeftArm.transform.rotate(mft::quat::rotation(mft::vec3(0.0f, 0.0f, 0.2f));
	//head.transform.rotate(mft::quat::rotation(mft::vec3(mft::radians(90.0f), 0.0f, 0.0f));

	Scene	scene;
	scene.drawGrid = true;

	std::shared_ptr<GLFont>	font = AssetManager::getInstance().loadAsset<GLFont>("resources/fonts/arial.ttf");

	uint32_t	newTime = 0;
	uint32_t	fpsCount = 0;
	uint32_t	frameTime = 0;
	uint32_t	fps = 0;

	uint32_t	moveTime = 0;

	std::cout << "Asset manager content:" << std::endl;
	AssetManager::getInstance().printContent();

	scene.setCameraSpeed(0.05f);

	scene.addObject(obj);
	scene.addObject(bobby);

	std::shared_ptr<Mesh> mesh(new Mesh(GLContext::cube));
	//mesh->transform.scale(mft::vec3(0.35f, 0.475f, 1.0f));
	mesh->setName("Mesh");
	mesh->setColor(mft::vec3(0.9f, 0.9f, 0.9f));
	mesh->addMesh(std::shared_ptr<Mesh>(new Mesh(GLContext::cube)));
	mesh->getChildren()[0]->setColor(mft::vec3(0.9f, 0.5f, 0.9f));
	mesh->getChildren()[0]->transform.move(mft::vec3(2.0f, 0.0f, 0.0f));
	mesh->getChildren()[0]->setName("Mesh 2");
	std::vector<std::shared_ptr<Mesh>> meshes;
	meshes.push_back(mesh);
	std::shared_ptr<GLObject>	object(new GLObject(meshes));
	object->setShader(GLContext::getShader("color"));

	scene.addObject(object);
	//bobby->visible = false;
	obj->visible = false;
	//obj->setAnimation(anim.get());
	scene.addLight(light1);

	InputState	mouseState = InputState::NRE_RELEASED;

	vec2i	mousePos, mouseStart;

	int frame = 0;

	scene.setLightingMode(LightingMode::Lit);

	std::shared_ptr<Mesh> selectedMesh = object->getMeshes()[0];
	//std::shared_ptr<Mesh> selectedMesh = bobby->getMeshes()[0]->getChildren()[2];

	while (running)
	{
		uint32_t	deltaTime = SDL_GetTicks() - moveTime;
		moveTime = SDL_GetTicks();
		if (mouseState == InputState::NRE_PRESS)
			mouseState = InputState::NRE_PRESSED;
		if (mouseState == InputState::NRE_RELEASE)
			mouseState = InputState::NRE_RELEASED;
		SDL_GetMouseState(&mousePos.x, &mousePos.y);
		if (mouseState == InputState::NRE_PRESSED)
		{
			scene.setYaw(scene.getYaw() + (float)(mouseStart.x - mousePos.x) * scene.getCamera().sensitivity);
			scene.setPitch(scene.getPitch() - (float)(mouseStart.y - mousePos.y) * scene.getCamera().sensitivity);
			mouseStart = mousePos;
		}
		while (SDL_PollEvent(&e))
		{
			switch (e.type)
			{
			case SDL_QUIT:
				running = 0;
				break;
			case SDL_KEYDOWN:
				if (e.key.keysym.sym == SDLK_ESCAPE)
					running = 0;
				if (e.key.keysym.sym == SDLK_LEFT)
				{
					//scene.left(deltaTime);
					selectedMesh->transform.move(mft::vec3(-0.05f, 0.0f, 0.0f));
					std::cout << selectedMesh->getName() << ": pos = ";
					std::cout << selectedMesh->transform.getPos();
					std::cout << ", scale = " << selectedMesh->transform.getScale() << std::endl;
					//std::cout << "Object matrix: " << character->transform.getMatrix() << std::endl;
					//std::cout << "Torso matrix: " << torso.transformMatrix << std::endl;
					//std::cout << "Head matrix: " << head.transformMatrix << std::endl;
					//obj->transform.move(mft::vec3(-1.0f, 0.0f, 0.0f));
				}
				if (e.key.keysym.sym == SDLK_RIGHT)
				{
					//scene.right(deltaTime);
					selectedMesh->transform.move(mft::vec3(0.05f, 0.0f, 0.0f));
					std::cout << selectedMesh->getName() << ": pos = ";
					std::cout << selectedMesh->transform.getPos();
					std::cout << ", scale = " << selectedMesh->transform.getScale() << std::endl;
					//std::cout << "Object matrix: " << character->transform.getMatrix() << std::endl;
					//std::cout << "Torso matrix: " << torso.transformMatrix << std::endl;
					//std::cout << "Head matrix: " << head.transformMatrix << std::endl;
					//obj->transform.move(mft::vec3(1.0f, 0.0f, 0.0f));
				}
				if (e.key.keysym.sym == SDLK_UP)
				{
					//scene.forward(deltaTime);
					selectedMesh->transform.move(mft::vec3(0.0f, 0.05f, 0.0f));
					std::cout << selectedMesh->getName() << ": pos = ";
					std::cout << selectedMesh->transform.getPos();
					std::cout << ", scale = " << selectedMesh->transform.getScale() << std::endl;
					//std::cout << "Object matrix: " << character->transform.getMatrix() << std::endl;
					//std::cout << "Torso matrix: " << torso.transformMatrix << std::endl;
					//std::cout << "Head matrix: " << head.transformMatrix << std::endl;
					//obj->transform.move(mft::vec3(0.0f, 0.0f, 1.0f));
				}
				if (e.key.keysym.sym == SDLK_DOWN)
				{
					//scene.backward(deltaTime);
					selectedMesh->transform.move(mft::vec3(0.0f, -0.05f, 0.0f));
					std::cout << selectedMesh->getName() << ": pos = ";
					std::cout << selectedMesh->transform.getPos();
					std::cout << ", scale = " << selectedMesh->transform.getScale() << std::endl;
					//std::cout << "Object matrix: " << character->transform.getMatrix() << std::endl;
					//std::cout << "Torso matrix: " << torso.transformMatrix << std::endl;
					//std::cout << "Head matrix: " << head.transformMatrix << std::endl;
					//obj->transform.move(mft::vec3(0.0f, 0.0f, -1.0f));
				}
				if (e.key.keysym.sym == SDLK_a)
				{
					scene.left(deltaTime / 2.0);
				}
				if (e.key.keysym.sym == SDLK_d)
				{
					scene.right(deltaTime / 2.0);
				}
				if (e.key.keysym.sym == SDLK_w)
				{
					scene.forward(deltaTime / 2.0);
				}
				if (e.key.keysym.sym == SDLK_s)
				{
					scene.backward(deltaTime / 2.0);
				}
				if (e.key.keysym.sym == SDLK_z)
				{
					if (scene.getDrawMode() != DrawMode::Wireframe)
						scene.setDrawMode(DrawMode::Wireframe);
					else
						scene.setDrawMode(DrawMode::Fill);
				}
				if (e.key.keysym.sym == SDLK_8)
				{
					//obj->transform.move(mft::vec3(-1.0f, 0.0f, 0.0f));
					//character->transform.rotate(mft::quat::rotation(mft::vec3(0.0f, 0.0f, -0.2f), mft::radians(90.0f)));
					selectedMesh->transform.scale(mft::vec3(1.05f, 1.0f, 1.0f));
					std::cout << selectedMesh->getName() << ": pos = ";
					std::cout << selectedMesh->transform.getPos();
					std::cout << ", scale = " << selectedMesh->transform.getScale() << std::endl;
				}
				if (e.key.keysym.sym == SDLK_6)
				{
					selectedMesh->transform.scale(mft::vec3(0.95f, 1.0f, 1.0f));
					std::cout << selectedMesh->getName() << ": pos = ";
					std::cout << selectedMesh->transform.getPos();
					std::cout << ", scale = " << selectedMesh->transform.getScale() << std::endl;
					//obj->transform.move(mft::vec3(1.0f, 0.0f, 0.0f));
					//character->transform.rotate(mft::quat::rotation(mft::vec3(0.0f, 0.0f, 0.2f), mft::radians(90.0f)));
				}
				if (e.key.keysym.sym == SDLK_4)
				{
					selectedMesh->transform.scale(mft::vec3(1.0f, 1.05f, 1.0f));
					std::cout << selectedMesh->getName() << ": pos = ";
					std::cout << selectedMesh->transform.getPos();
					std::cout << ", scale = " << selectedMesh->transform.getScale() << std::endl;
					//obj->transform.move(mft::vec3(0.0f, 0.0f, 1.0f));
					//character->transform.rotate(mft::quat::rotation(mft::vec3(0.0f, 0.2f, 0.0f), mft::radians(90.0f)));
				}
				if (e.key.keysym.sym == SDLK_2)
				{
					selectedMesh->transform.scale(mft::vec3(1.0f, 0.95f, 1.0f));
					std::cout << selectedMesh->getName() << ": pos = ";
					std::cout << selectedMesh->transform.getPos();
					std::cout << ", scale = " << selectedMesh->transform.getScale() << std::endl;
					//obj->transform.move(mft::vec3(0.0f, 0.0f, -1.0f));
					//character->transform.rotate(mft::quat::rotation(mft::vec3(0.0f, -0.2f, 0.0f), mft::radians(90.0f)));
				}
				if (e.key.keysym.sym == SDLK_KP_4)
				{
					selectedMesh->transform.rotate(mft::quat::rotation(mft::vec3(0.0f, 0.0f, 1.0f), mft::radians(10.0f)));
					std::cout << selectedMesh->getName() << ": rotation = ";
					std::cout << selectedMesh->transform.getRotation() << std::endl;
				}
				if (e.key.keysym.sym == SDLK_KP_6)
				{
					selectedMesh->transform.rotate(mft::quat::rotation(mft::vec3(0.0f, 0.0f, 1.0f), mft::radians(-10.0f)));
					std::cout << selectedMesh->getName() << ": rotation = ";
					std::cout << selectedMesh->transform.getRotation() << std::endl;
				}
				if (e.key.keysym.sym == SDLK_KP_8)
				{
					selectedMesh->transform.rotate(mft::quat::rotation(mft::vec3(0.0f, 1.0f, 0.0f), mft::radians(10.0f)));
					std::cout << selectedMesh->getName() << ": rotation = ";
					std::cout << selectedMesh->transform.getRotation() << std::endl;
				}
				if (e.key.keysym.sym == SDLK_KP_2)
				{
					selectedMesh->transform.rotate(mft::quat::rotation(mft::vec3(0.0f, 1.0f, 0.0f), mft::radians(-10.0f)));
					std::cout << selectedMesh->getName() << ": rotation = ";
					std::cout << selectedMesh->transform.getRotation() << std::endl;
				}
				if (e.key.keysym.sym == SDLK_p)
				{
					std::cout << "Animation status = ";
					std::shared_ptr<GLObject> object;
					std::shared_ptr<Animation> animation;
					if (mode == Object || mode == Bones)
					{
						object = obj;
						animation = anim;
					}
					else if (mode == Bob)
					{
						object = bobby;
						animation = bobbyWalking;
					}
					AnimationState animState = object->getAnimationState();
					if (animState == AnimationState::Stopped)
					{
						object->playAnimation(animation.get());
						std::cout << " started " << std::endl;
					}
					else if (animState == AnimationState::Playing)
					{
						object->pauseAnimation();
						std::cout << " pause " << std::endl;
					}
					else if (animState == AnimationState::Paused)
					{
						object->resumeAnimation();
						std::cout << " resumed " << std::endl;
					}
				}
				if (e.key.keysym.sym == SDLK_l)
				{
					if (scene.getLightingMode() == LightingMode::Lit)
						scene.setLightingMode(LightingMode::Unlit);
					else
						scene.setLightingMode(LightingMode::Lit);
				}
				if (e.key.keysym.sym == SDLK_o)
				{
					mode = Object;
					if (scene.getLightingMode() == LightingMode::Lit)
						obj->setShader(context.getShader("default"));
					else
						obj->setShader(context.getShader("unlit"));
					obj->bindBones();
				}
				if (e.key.keysym.sym == SDLK_v)
				{
					mode = Bones;
					obj->setShader(context.getShader("bonesInfluence"));
				}
				if (e.key.keysym.sym == SDLK_b)
				{
					mode = Bob;
					obj->visible = obj->visible == true ? false : true;
					bobby->visible = obj->visible == true ? false : true;
				}
				if (e.key.keysym.sym == SDLK_r)
				{
					if (mode == Object || mode == Bones)
						obj->resetPose();
					else if (mode == Bob)
						bobby->resetPose();
				}
				if (e.key.keysym.sym == SDLK_KP_PLUS)
				{
					selectedBone++;
					if (selectedBone > obj->getNbBones())
						selectedBone = 0;
					bindInt(context.getShader("bonesInfluence")->programID, "selectedBone", selectedBone);
					frame += 1;
					obj->setToKeyFrame(frame);
					if (selectedMesh == object->getMeshes()[0])
					{
						selectedMesh = selectedMesh->getChildren()[0];
						std::cout << "Child selected " << std::endl;
					}
					else if (selectedMesh == object->getMeshes()[0]->getChildren()[0])
					{
						selectedMesh = object->getMeshes()[0];
						std::cout << "Main selected " << std::endl;
					}
				}
				if (e.key.keysym.sym == SDLK_KP_MINUS)
				{
					selectedBone--;
					if (selectedBone < 0)
						selectedBone = obj->getNbBones() - 1;
					bindInt(context.getShader("bonesInfluence")->programID, "selectedBone", selectedBone);
					frame -= 1;
					if (frame < 0)
						frame = 0;
					obj->setToKeyFrame(frame);
				}
				break;
				case SDL_MOUSEBUTTONDOWN:
					if (e.button.button == SDL_BUTTON_LEFT)
					{
						if (mouseState == InputState::NRE_RELEASED)
						{
							mouseState = InputState::NRE_PRESS;
							mouseStart = mousePos;
							SDL_ShowCursor(SDL_DISABLE);
						}
					}
					else if (e.button.button == SDL_BUTTON_RIGHT)
					{

					}
					break;
				case SDL_MOUSEBUTTONUP:
					if (e.button.button == SDL_BUTTON_LEFT)
					{
						if (mouseState == InputState::NRE_PRESSED)
						{
							mouseState = InputState::NRE_RELEASE;
							SDL_ShowCursor(SDL_ENABLE);
						}
					}
					else if (e.button.button == SDL_BUTTON_RIGHT)
					{

					}
					break;
			}
		}
		newTime = SDL_GetTicks();
		fpsCount++;
		if (newTime - frameTime >= 1000)
		{
			frameTime = newTime;
			fps = fpsCount;
			fpsCount = 0;
		}
		GLCallThrow(glClearColor, 0.2f, 0.2f, 0.2f, 1.0f);
		GLCallThrow(glClear, GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		font->RenderText(context.getShader("text"), std::to_string(fps), mft::vec2(0, 0), 1, mft::vec3(1.0, 1.0, 1.0));
		font->RenderText(context.getShader("text"), std::string("Frame = " + std::to_string(frame)), mft::vec2(600, 800), 1, mft::vec3(1.0, 1.0, 1.0));

		scene.render();
		scene.renderBones();
		context.swapWindow();
	}
	return 0;
}
