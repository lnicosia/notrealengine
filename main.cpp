#include "SDL.h"
#include "mft/mft.hpp"
#include "notrealengine/notrealengine.hpp"
#include "Object/GLObject.class.hpp"
#include "Object/Animation.class.hpp"
#include "Object/TextureLoader.class.hpp"
#include "Object/AssetManager.class.hpp"
#include "Object/Light.class.hpp"
#include "Object/Scene.class.hpp"
#include "GLShaderProgram.class.hpp"
#include "TextRendering/GLFont.class.hpp"
#include "Camera.class.hpp"
#include "Inputs/SDLEvents.class.hpp"
#include <vector>

enum DrawMode
{
	Object,
	Bob,
	Bones
};

using namespace notrealengine;

std::shared_ptr<GLObject>	InitBobby(void);

int		main(int ac, char** av)
{
	if (ac < 2)
		return 1;

	SDLWindow window("Not real engine", std::pair<int, int>(1600, 900));
	GLContext_SDL	context(window.getContext(), window.getWindow());
	context.makeCurrent();
	SDL_Event	e;
	int	running = 1;
	int	mode = Object;
	int selectedBone = 0;

	std::shared_ptr<GLObject>	obj = AssetManager::getInstance().loadAsset<GLObject>(av[1]);
	std::shared_ptr<Animation>	anim = AssetManager::getInstance().loadAsset<Animation>(av[1], 0);
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

	Mesh& selectedMesh = (*(*bobby->getMeshes()[0]).getChildren()[2]);


	//upperLeftArm.transform.rotate(mft::quat::rotation(mft::vec3(0.0f, 0.0f, 0.2f));
	//head.transform.rotate(mft::quat::rotation(mft::vec3(mft::radians(90.0f), 0.0f, 0.0f));

	Scene	scene;

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
	bobby->visible = false;
	scene.addLight(light1);

	InputState	mouseState = InputState::NRE_RELEASED;

	vec2i	mousePos, mouseStart;

	int frame = 0;

	scene.setRenderingMode(RenderingMode::Unlit);

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
					selectedMesh.transform.move(mft::vec3(-0.05f, 0.0f, 0.0f));
					std::cout << selectedMesh.getName() << ": pos = ";
					std::cout << selectedMesh.transform.getPos();
					std::cout << ", scale = " << selectedMesh.transform.getScale() << std::endl;
					//std::cout << "Object matrix: " << character->transform.getMatrix() << std::endl;
					//std::cout << "Torso matrix: " << torso.transformMatrix << std::endl;
					//std::cout << "Head matrix: " << head.transformMatrix << std::endl;
					//obj->transform.move(mft::vec3(-1.0f, 0.0f, 0.0f));
				}
				if (e.key.keysym.sym == SDLK_RIGHT)
				{
					//scene.right(deltaTime);
					selectedMesh.transform.move(mft::vec3(0.05f, 0.0f, 0.0f));
					std::cout << selectedMesh.getName() << ": pos = ";
					std::cout << selectedMesh.transform.getPos();
					std::cout << ", scale = " << selectedMesh.transform.getScale() << std::endl;
					//std::cout << "Object matrix: " << character->transform.getMatrix() << std::endl;
					//std::cout << "Torso matrix: " << torso.transformMatrix << std::endl;
					//std::cout << "Head matrix: " << head.transformMatrix << std::endl;
					//obj->transform.move(mft::vec3(1.0f, 0.0f, 0.0f));
				}
				if (e.key.keysym.sym == SDLK_UP)
				{
					//scene.forward(deltaTime);
					selectedMesh.transform.move(mft::vec3(0.0f, 0.05f, 0.0f));
					std::cout << selectedMesh.getName() << ": pos = ";
					std::cout << selectedMesh.transform.getPos();
					std::cout << ", scale = " << selectedMesh.transform.getScale() << std::endl;
					//std::cout << "Object matrix: " << character->transform.getMatrix() << std::endl;
					//std::cout << "Torso matrix: " << torso.transformMatrix << std::endl;
					//std::cout << "Head matrix: " << head.transformMatrix << std::endl;
					//obj->transform.move(mft::vec3(0.0f, 0.0f, 1.0f));
				}
				if (e.key.keysym.sym == SDLK_DOWN)
				{
					//scene.backward(deltaTime);
					selectedMesh.transform.move(mft::vec3(0.0f, -0.05f, 0.0f));
					std::cout << selectedMesh.getName() << ": pos = ";
					std::cout << selectedMesh.transform.getPos();
					std::cout << ", scale = " << selectedMesh.transform.getScale() << std::endl;
					//std::cout << "Object matrix: " << character->transform.getMatrix() << std::endl;
					//std::cout << "Torso matrix: " << torso.transformMatrix << std::endl;
					//std::cout << "Head matrix: " << head.transformMatrix << std::endl;
					//obj->transform.move(mft::vec3(0.0f, 0.0f, -1.0f));
				}
				if (e.key.keysym.sym == SDLK_a)
				{
					scene.left(deltaTime);
				}
				if (e.key.keysym.sym == SDLK_d)
				{
					scene.right(deltaTime);
				}
				if (e.key.keysym.sym == SDLK_w)
				{
					scene.forward(deltaTime);
				}
				if (e.key.keysym.sym == SDLK_s)
				{
					scene.backward(deltaTime);
				}
				if (e.key.keysym.sym == SDLK_4)
				{
					//obj->transform.move(mft::vec3(-1.0f, 0.0f, 0.0f));
					//character->transform.rotate(mft::quat::rotation(mft::vec3(0.0f, 0.0f, -0.2f), mft::radians(90.0f)));
					selectedMesh.transform.scale(mft::vec3(1.05f, 1.0f, 1.0f));
					std::cout << selectedMesh.getName() << ": pos = ";
					std::cout << selectedMesh.transform.getPos();
					std::cout << ", scale = " << selectedMesh.transform.getScale() << std::endl;
				}
				if (e.key.keysym.sym == SDLK_6)
				{
					selectedMesh.transform.scale(mft::vec3(0.95f, 1.0f, 1.0f));
					std::cout << selectedMesh.getName() << ": pos = ";
					std::cout << selectedMesh.transform.getPos();
					std::cout << ", scale = " << selectedMesh.transform.getScale() << std::endl;
					//obj->transform.move(mft::vec3(1.0f, 0.0f, 0.0f));
					//character->transform.rotate(mft::quat::rotation(mft::vec3(0.0f, 0.0f, 0.2f), mft::radians(90.0f)));
				}
				if (e.key.keysym.sym == SDLK_8)
				{
					selectedMesh.transform.scale(mft::vec3(1.0f, 1.05f, 1.0f));
					std::cout << selectedMesh.getName() << ": pos = ";
					std::cout << selectedMesh.transform.getPos();
					std::cout << ", scale = " << selectedMesh.transform.getScale() << std::endl;
					//obj->transform.move(mft::vec3(0.0f, 0.0f, 1.0f));
					//character->transform.rotate(mft::quat::rotation(mft::vec3(0.0f, 0.2f, 0.0f), mft::radians(90.0f)));
				}
				if (e.key.keysym.sym == SDLK_2)
				{
					selectedMesh.transform.scale(mft::vec3(1.0f, 0.95f, 1.0f));
					std::cout << selectedMesh.getName() << ": pos = ";
					std::cout << selectedMesh.transform.getPos();
					std::cout << ", scale = " << selectedMesh.transform.getScale() << std::endl;
					//obj->transform.move(mft::vec3(0.0f, 0.0f, -1.0f));
					//character->transform.rotate(mft::quat::rotation(mft::vec3(0.0f, -0.2f, 0.0f), mft::radians(90.0f)));
				}
				if (e.key.keysym.sym == SDLK_KP_4)
				{
					selectedMesh.transform.rotate(mft::quat::rotation(mft::vec3(0.0f, 0.0f, 1.0f), mft::radians(10.0f)));
					std::cout << selectedMesh.getName() << ": rotation = ";
					std::cout << selectedMesh.transform.getRotation() << std::endl;
				}
				if (e.key.keysym.sym == SDLK_KP_6)
				{
					selectedMesh.transform.rotate(mft::quat::rotation(mft::vec3(0.0f, 0.0f, 1.0f), mft::radians(-10.0f)));
					std::cout << selectedMesh.getName() << ": rotation = ";
					std::cout << selectedMesh.transform.getRotation() << std::endl;
				}
				if (e.key.keysym.sym == SDLK_KP_8)
				{
					selectedMesh.transform.rotate(mft::quat::rotation(mft::vec3(0.0f, 1.0f, 0.0f), mft::radians(10.0f)));
					std::cout << selectedMesh.getName() << ": rotation = ";
					std::cout << selectedMesh.transform.getRotation() << std::endl;
				}
				if (e.key.keysym.sym == SDLK_KP_2)
				{
					selectedMesh.transform.rotate(mft::quat::rotation(mft::vec3(0.0f, 1.0f, 0.0f), mft::radians(-10.0f)));
					std::cout << selectedMesh.getName() << ": rotation = ";
					std::cout << selectedMesh.transform.getRotation() << std::endl;
				}
				if (e.key.keysym.sym == SDLK_l)
				{
					if (scene.getRenderingMode() == RenderingMode::Lit)
						scene.setRenderingMode(RenderingMode::Unlit);
					else if (scene.getRenderingMode() == RenderingMode::Unlit)
						scene.setRenderingMode(RenderingMode::Lit);
				}
				if (e.key.keysym.sym == SDLK_o)
				{
					mode = Object;
					obj->setShader(context.getShader("default"));
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
					obj->resetPose();
					std::cout << "Pose reset " << std::endl;
				}
				if (e.key.keysym.sym == SDLK_KP_PLUS)
				{
					selectedBone++;
					if (selectedBone > obj->getNbBones())
						selectedBone = 0;
					bindInt(context.getShader("bonesInfluence")->programID, "selectedBone", selectedBone);
					frame++;
					anim->playAnimation(*obj, frame);
				}
				if (e.key.keysym.sym == SDLK_KP_MINUS)
				{
					selectedBone--;
					if (selectedBone < 0)
						selectedBone = obj->getNbBones() - 1;
					bindInt(context.getShader("bonesInfluence")->programID, "selectedBone", selectedBone);
					frame--;
					anim->playAnimation(*obj, frame);
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
		font->RenderText(context.getShader("text"), std::string("Selected bone = " + std::to_string(selectedBone)), mft::vec2(600, 800), 1, mft::vec3(1.0, 1.0, 1.0));

		scene.render();
		scene.renderBones();
		context.swapWindow();
	}
	return 0;
}

/*#include "SDLWindow.class.hpp"
#include "GLContext_SDL.class.hpp"
#include "TextRendering/GLFont.class.hpp"
#include "Object/TextureLoader.class.hpp"
#include "UI/UIManager.class.hpp"
#include "UI/Button.class.hpp"

using namespace notrealengine;

std::string	str = "Num " + std::to_string(0);
int	value = 0;

int		updateText(void)
{
	value++;
	str = "Num " + std::to_string(value);
	return 0;
}

int		main()
{
	SDLWindow window("Not real engine", std::pair<int, int>(1600, 900));
	GLContext_SDL	context(window.getContext(), window.getWindow());
	context.registerShader("color", "shaders/test.vs", "shaders/colorShader.fs");
	context.registerShader("bones", "shaders/anim.vs", "shaders/bones.fs");
	context.registerShader("anim", "shaders/anim.vs", "shaders/test.fs");
	context.registerShader("default", "shaders/anim.vs", "shaders/test.fs");

	context.registerShader("2d", "shaders/2d.vs", "shaders/2d.fs");
	context.makeCurrent();

	GLFont	font("resources/fonts/arial.ttf");
	mft::mat4	textProject = mft::mat4::ortho(0.0f, 1600.0f, 0.0f, 900.0f);
	GLCallThrow(glUseProgram, context.getShader("2d")->programID);
	GLCallThrow(glUniformMatrix4fv, GLCallThrow(glGetUniformLocation, context.getShader("2d")->programID, "projection"), 1, GL_TRUE, static_cast<float*>(textProject));

	int	running = 1;
	SDL_Event	e;

	uint32_t	newTime = 0;
	uint32_t	fpsCount = 0;
	uint32_t	frameTime = 0;
	uint32_t	fps = 0;

	glEnable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	UIManager	ui;
	Button		testButton(context.getShader("2d"));
	testButton.setPos(mft::vec2i(800, 450));
	std::shared_ptr<GLFont>	fontPtr(new GLFont("resources/fonts/arial.ttf"));
	testButton.setFont(fontPtr);
	testButton.setText("Coucou");
	testButton.setFontColor(mft::vec3(0.0f, 0.0f, 0.0f));
	testButton.setReleaseFunc(updateText);
	int i = 0;
	std::shared_ptr<Button>		buttonPtr(new Button(testButton));

	Button		testButton2(context.getShader("2d"));
	testButton2.setFont(fontPtr);
	testButton2.setFontColor(mft::vec3(0.0f, 0.0f, 0.0f));
	testButton2.setPos(mft::vec2i(800, 550));
	testButton2.setText(str);
	//testButton2.setText("Bouton de test");
	std::shared_ptr<Button>		buttonPtr2(new Button(testButton2));
	//UIElement	testUI(context.getShader("2d"));
	//testUI.setPos(mft::vec2(800, 450));
	//std::shared_ptr<UIElement>		uiPtr(new UIElement(testUI));

	ui.registerElement(buttonPtr);
	ui.registerElement(buttonPtr2);

	mft::vec2i	mousePos;
	InputState	mouseState = InputState::NRE_RELEASED;

	while (running)
	{
		if (mouseState == InputState::NRE_PRESS)
			mouseState = InputState::NRE_PRESSED;
		if (mouseState == InputState::NRE_RELEASE)
			mouseState = InputState::NRE_RELEASED;
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
			case SDL_MOUSEBUTTONDOWN:
				mouseState = InputState::NRE_PRESS;
				break;
			case SDL_MOUSEBUTTONUP:
				mouseState = InputState::NRE_RELEASE;
				break;
			}
		}
		SDL_GetMouseState(&mousePos.x, &mousePos.y);
		newTime = SDL_GetTicks();
		fpsCount++;
		if (newTime - frameTime >= 1000)
		{
			frameTime = newTime;
			fps = fpsCount;
			fpsCount = 0;
		}
		GLCallThrow(glClearColor, 0.05f, 0.05f, 0.05f, 1.0f);
		GLCallThrow(glClear, GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		buttonPtr2->setText(str);
		font.RenderText(context.getShader("2d"), std::to_string(fps), mft::vec2i(10, 10), 1, mft::vec3(1.0, 1.0, 1.0));
		font.RenderText(context.getShader("2d"),
			"Mouse pos: [" + std::to_string(mousePos.x) + ", " + std::to_string(mousePos.y) + "]",
			mft::vec2i(650, 10), 1, mft::vec3(1.0, 1.0, 1.0));
		ui.update(mousePos, mouseState);
		context.swapWindow();
	}
	return 0;
}*/
