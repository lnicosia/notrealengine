#ifndef _BUTTON_CLASS_H_
#define _BUTTON_CLASS_H_

#include "Inputs/Binding.class.hpp"
#include "Object/Texture.class.hpp"
#include "Object/AssetManager.class.hpp"
#include "mft/mft.hpp"
#include "UI/UIElement.class.hpp"
#include "TextRendering/GLFont.class.hpp"

#include <functional>

namespace notrealengine
{
	class Button: public UIElement
	{
	public:
		Button(GLShaderProgram* shader,
			std::shared_ptr<Texture> imgReleased = AssetManager::getInstance().loadAsset<Texture>("resources/UI/defaultUI-released.png", "UI"),
			std::shared_ptr<Texture> imgPressed = AssetManager::getInstance().loadAsset<Texture>("resources/UI/defaultUI-pressed.png", "UI"),
			std::shared_ptr<Texture> imgHovered = AssetManager::getInstance().loadAsset<Texture>("resources/UI/defaultUI-hovered.png", "UI"),
			mft::vec2i pos = mft::vec2i(0, 0));
		~Button();

		void
			draw() const;

		/** Update the button's state according to inputs
		 */
		virtual void
			update(const mft::vec2i& mousePos, const InputState inputState);

		//	Accessors

		const std::string&
			getText() const;

		//	Setters

		void
			setHovered(std::shared_ptr<Texture> img, mft::vec2i size,
				std::function<int> func);
		void
			setPressed(std::shared_ptr<Texture> img, mft::vec2i size,
				std::function<int> func);
		void
			setReleased(std::shared_ptr<Texture> img, mft::vec2i size,
				std::function<int> func);

		//	Set the image and size for each state

		void
			setHoveredStyle(std::shared_ptr<Texture> img, mft::vec2i size);
		void
			setPressedStyle(std::shared_ptr<Texture> img, mft::vec2i size);
		void
			setReleasedStyle(std::shared_ptr<Texture> img, mft::vec2i size);


		//	Set the image for each state

		void
			setHoveredImg(std::shared_ptr<Texture> img);
		void
			setPressedImg(std::shared_ptr<Texture> img);
		void
			setReleasedImg(std::shared_ptr<Texture> img);

		//	Set the size for each state

		void
			setHoveredSize(std::shared_ptr<Texture> img, mft::vec2i size);
		void
			setPressedSize(std::shared_ptr<Texture> img, mft::vec2i size);
		void
			setReleasedSize(std::shared_ptr<Texture> img, mft::vec2i size);

		//	Set the function for each state

		void
			setHoveredFunc(std::function<int> func);
		void
			setPressedFunc(std::function<int>& func);
		void
			setPressFunc(std::function<int>& func);
		void
			setReleaseFunc(const std::function<int(void)>& func);

		void
			setText(const std::string& text);

		void
			setFont(const std::shared_ptr<GLFont>& font);
		void
			setFontColor(const mft::vec3& color);

		virtual void
			setPos(const mft::vec2i&& newPos);

	private:
		InputState	state;

		std::shared_ptr<Texture>	imgReleased;
		std::shared_ptr<Texture>	imgPressed;
		std::shared_ptr<Texture>	imgHovered;
		mft::vec2i					sizeReleased;
		mft::vec2i					sizePressed;
		mft::vec2i					sizeHovered;
		std::function<int(void)>	funcHovered;
		std::function<int(void)>	funcRelease;
		std::function<int(void)>	funcPressed;
		std::function<int(void)>	funcPress;

		std::string	text;
		mft::vec2i	textPos;
		float		textScale;
		std::shared_ptr<GLFont>	font;
		mft::vec3	fontColor;

		void
			updateDrawData();
	};

}

#endif
