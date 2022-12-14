#ifndef _BUTTON_CLASS_H_
#define _BUTTON_CLASS_H_

#include "Inputs/Action.class.hpp"
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
		Button(mft::vec2i pos = mft::vec2i(0, 0),
		std::shared_ptr<Texture> imgReleased =
		AssetManager::getInstance().loadAsset<Texture>("resources/UI/defaultUI-released.png", "UI"),
		std::shared_ptr<Texture> imgPressed =
		AssetManager::getInstance().loadAsset<Texture>("resources/UI/defaultUI-pressed.png", "UI"),
		std::shared_ptr<Texture> imgHovered =
		AssetManager::getInstance().loadAsset<Texture>("resources/UI/defaultUI-hovered.png", "UI"),
		GLShaderProgram* shader = GLContext::getShader("2d"));
		~Button();

		void
			draw() const override;

		/** Update the button's state according to inputs
		 */
		void
			update(const mft::vec2i& mousePos, const InputState inputState) override;

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
			setHoveredSize(std::shared_ptr<Texture> img, const mft::vec2i&& size);
		void
			setPressedSize(std::shared_ptr<Texture> img, const mft::vec2i&& size);
		void
			setReleasedSize(std::shared_ptr<Texture> img, const mft::vec2i&& size);
		void
			setAllSizes(const mft::vec2i&& size);
		void
			setSize(const mft::vec2i&& newSize) override;

		//	Set the function for each state

		void
			setWhenHovered(std::shared_ptr<ActionWrapper> func);
		void
			setWhenPressed(std::shared_ptr<ActionWrapper> func);
		void
			setOnPress(std::shared_ptr<ActionWrapper> func);
		void
			setOnRelease(std::shared_ptr<ActionWrapper> func);

		void
			setText(const std::string& text);

		void
			setFont(const std::shared_ptr<GLFont>& font);
		void
			setTextColor(const mft::vec4& color);

		void
			setPos(const mft::vec2i&& newPos) override;

		void
			addPos(const mft::vec2i& pos) override;

		std::shared_ptr<ActionWrapper>	whenHovered;
		std::shared_ptr<ActionWrapper>	whenPressed;
		std::shared_ptr<ActionWrapper>	onRelease;
		std::shared_ptr<ActionWrapper>	onPress;

	private:
		InputState	state;

		std::shared_ptr<Texture>	imgReleased;
		std::shared_ptr<Texture>	imgPressed;
		std::shared_ptr<Texture>	imgHovered;
		mft::vec2i		sizeReleased;
		mft::vec2i		sizePressed;
		mft::vec2i		sizeHovered;

		std::string	text;
		mft::vec2i	textPos;
		float		textSize;
		std::shared_ptr<GLFont>	font;
		mft::vec4	textColor;

		/**	Update the position of relative elements, like the text
		*/
		void
			updateDrawData();
	};

}

#endif
