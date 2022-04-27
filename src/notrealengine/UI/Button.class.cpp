#include "UI/Button.class.hpp"
#include "UsingExternalLibs.hpp"
#include "NotRealEngine.hpp"

namespace notrealengine
{
	Button::Button(mft::vec2i pos,
		std::shared_ptr<Texture> imgReleased,
		std::shared_ptr<Texture> imgPressed,
		std::shared_ptr<Texture> imgHovered,
		GLShaderProgram* shader)
		:
		UIElement(pos, imgReleased, shader),
		state(InputState::NRE_RELEASED),
		text(""), textPos(mft::vec2i()), textSize(16.0f), textColor(mft::vec4(1.0f)),
		sizeReleased(image->getSize()),
		sizePressed(image->getSize()),
		sizeHovered(image->getSize()),
		imgReleased(imgReleased), imgPressed(imgPressed), imgHovered(imgHovered),
		whenHovered(), whenPressed(), onRelease(), onPress()
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

	void	Button::setAllSizes(const mft::vec2i&& size)
	{
		this->sizeReleased = size;
		this->sizePressed = size;
		this->sizeHovered = size;
		this->size = size;
		updateDrawData();
	}

	void	Button::setSize(const mft::vec2i&& size)
	{
		setAllSizes(std::move(size));
	}

	void	Button::setWhenHovered(std::shared_ptr<ActionWrapper> func)
	{
		this->whenHovered = func;
	}

	void	Button::setWhenPressed(std::shared_ptr<ActionWrapper> func)
	{
		this->whenPressed = func;
	}

	void	Button::setOnPress(std::shared_ptr<ActionWrapper> func)
	{
		this->onPress = func;
	}

	void	Button::setOnRelease(std::shared_ptr<ActionWrapper> func)
	{
		this->onRelease = func;
	}

	void	Button::setHoveredImg(std::shared_ptr<Texture> img)
	{
		this->imgHovered = img;
	}

	void	Button::setPressedImg(std::shared_ptr<Texture> img)
	{
		this->imgPressed = img;
	}

	void	Button::setReleasedImg(std::shared_ptr<Texture> img)
	{
		this->imgReleased = img;
	}

	void	Button::setPos(const mft::vec2i&& newPos)
	{
		pos = newPos;
		updateDrawData();
	}

	void	Button::addPos(const mft::vec2i& pos)
	{
		this->pos += pos;
		for (auto child : this->children)
		{
			child->addPos(pos);
		}
		updateDrawData();
	}

	void	Button::updateDrawData()
	{
		if (!font)
			return;
#ifdef USING_EXTERNAL_LIBS
		mft::vec2i characterSize = font->getCharacter('C')->getSize();
#else
		mft::vec2i characterSize = font->getCharacterSize('C');
#endif

		float textWidth = 0.0f;
		for (char c : text)
		{
			textWidth += font->getCharacterSize(c).x;
		}
		mft::vec2 textSize(textWidth, characterSize.y);
		mft::vec2 scale;
		scale.x = (this->size.x * 0.8) / (float)textSize.x;
		scale.y = (this->size.y * 0.8) / (float)textSize.y;
		mft::vec2 size(0.8 * this->size.x / this->text.length(),
			0.8 * this->size.y);
		this->textSize = characterSize.y * std::min(scale.x, scale.y);
		textSize *= std::min(scale.x, scale.y);
		this->textPos = mft::vec2i(this->pos.x + this->size.x / 2 - textSize.x / 2,
			this->pos.y + this->size.y / 2 - textSize.y / 2 + 2);

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
			//std::cout << "Rendering text " << text << " at button pos " << this->pos << " text pos = " << textPos << " of size " << textSize << std::endl;
			font->RenderText(text, textPos, textSize, textColor);
		}
	}

	void	Button::update(const mft::vec2i& mousePos, const InputState mouseState)
	{
		if (active == false)
			return;
		switch (mouseState)
		{
		case InputState::NRE_PRESS:
			if (mousePos.x >= pos.x && mousePos.x <= pos.x + sizeReleased.x
				&& mousePos.y >= pos.y && mousePos.y <= pos.y + sizeReleased.y)
			{
				state = InputState::NRE_PRESSED;
				isMouseOnUI = true;
				if (onPress)
					onPress->execute();
			}
			break;
		case InputState::NRE_PRESSED:
			if (state == InputState::NRE_PRESSED)
			{
				if (whenPressed)
					whenPressed->execute();
			}
			break;
		case InputState::NRE_RELEASE:
			if (state == InputState::NRE_PRESSED)
			{
				state = InputState::NRE_RELEASED;
				if (onRelease)
					onRelease->execute();
			}
			break;
		case InputState::NRE_RELEASED:
			if (mousePos.x >= pos.x && mousePos.x <= pos.x + sizeReleased.x
				&& mousePos.y >= pos.y && mousePos.y <= pos.y + sizeReleased.y)
			{
				state = InputState::NRE_HOVERED;
				isMouseOnUI = true;
				if (whenHovered)
					whenHovered->execute();
			}
			else
				state = InputState::NRE_RELEASED;
			break;
		default:
			break;
		}
	}
}
