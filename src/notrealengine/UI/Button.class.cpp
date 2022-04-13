#include "UI/Button.class.hpp"
#include "UsingExternalLibs.hpp"

namespace notrealengine
{
	Button::Button(GLShaderProgram* shader,
		std::shared_ptr<Texture> imgReleased,
		std::shared_ptr<Texture> imgPressed,
		std::shared_ptr<Texture> imgHovered,
		mft::vec2i pos)
		:
		UIElement(shader, imgReleased, pos),
		state(InputState::NRE_RELEASED),
		text(""), textPos(mft::vec2i()), textScale(1.0f), textColor(mft::vec4(1.0f)),
		sizeReleased(image->getSize()),
		sizePressed(image->getSize()),
		sizeHovered(image->getSize()),
		imgReleased(imgReleased), imgPressed(imgPressed), imgHovered(imgHovered),
		funcHovered(), funcPressed(), funcPress(), funcRelease()
	{
	}

	Button::~Button()
	{
	}

	//	Accessors

	const std::string& Button::getText() const
	{
		return text;
	}

	//	Setters

	void	Button::setText(const std::string& text)
	{
		this->text = text;
		updateDrawData();
	}

	void	Button::setFont(const std::shared_ptr<GLFont>& font)
	{
		this->font = font;
		updateDrawData();
	}

	void	Button::setTextColor(const mft::vec4& color)
	{
		this->textColor = color;
	}

	void	Button::setPos(const mft::vec2i&& newPos)
	{
		pos = newPos;
		updateDrawData();
	}

	void	Button::setReleaseFunc(const std::function<int(void)>& func)
	{
		funcRelease = func;
	}

	void	Button::updateDrawData()
	{
		if (!font)
			return;
#ifdef USING_EXTERNAL_LIBS
		mft::vec2i size(font->getCharacter('C')->getSize().x * text.length(),
			font->getCharacter('C')->getSize().y);
#else
		mft::vec2i size(font->getCharacterSize('C').x * text.length(),
			font->getCharacterSize('C').y);
#endif
		mft::vec2 scale;
		if (size.x > this->size.x * 0.75)
			scale.x = (this->size.x * 0.75) / size.x;
		if (size.y > this->size.y * 0.75)
			scale.y = (this->size.y * 0.75) / size.y;
		textScale = scale.x < scale.y ? scale.x : scale.y;
		size.x *= textScale;
		size.y *= textScale;
		textPos = mft::vec2i(this->pos.x + this->size.x / 2 - size.x / 2,
			this->pos.y + this->size.y / 2 - size.y / 2);
	}

	void	Button::draw() const
	{
		if (visible == false)
			return;
		switch (state)
		{
		case InputState::NRE_HOVERED:
			imgHovered->draw(pos, sizeHovered, 0.0f, mft::vec4(1.0f));
			break;
		case InputState::NRE_PRESS:
		case InputState::NRE_PRESSED:
			imgPressed->draw(pos, sizePressed, 0.0f, mft::vec4(1.0f));
			break;
		case InputState::NRE_RELEASE:
		case InputState::NRE_RELEASED:
			imgReleased->draw(pos, sizeReleased, 0.0f, mft::vec4(1.0f));
			break;
		}
		if (text != "" && font != nullptr)
		{
			font->RenderText(text, textPos, textScale, textColor);
		}
	}

	void	Button::update(const mft::vec2i& mousePos, const InputState mouseState)
	{
		if (active == false)
			return;
		switch (mouseState)
		{
		case InputState::NRE_PRESS:
			if (mousePos.x >= pos.x && mousePos.x <= pos.x + sizePressed.x
				&& mousePos.y >= pos.y - sizeReleased.y && mousePos.y <= pos.y)
			{
				state = InputState::NRE_PRESSED;
				if (funcPress)
					funcPress();
			}
			break;
		case InputState::NRE_PRESSED:
			if (state == InputState::NRE_PRESSED)
			{
				if (funcPressed)
					funcPressed();
			}
			break;
		case InputState::NRE_RELEASE:
			if (state == InputState::NRE_PRESSED)
			{
				state = InputState::NRE_RELEASED;
				if (funcRelease)
					funcRelease();
			}
			break;
		case InputState::NRE_RELEASED:
			if (mousePos.x >= pos.x && mousePos.x <= pos.x + sizeReleased.x
				&& mousePos.y >= pos.y - sizeReleased.y && mousePos.y <= pos.y)
			{
				state = InputState::NRE_HOVERED;
				if (funcHovered)
					funcHovered();
			}
			else
				state = InputState::NRE_RELEASED;
			break;
		default:
			break;
		}
	}
}
