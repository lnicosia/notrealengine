#include "UI/UIElement.class.hpp"

namespace notrealengine
{
	UIElement::UIElement(mft::vec2i pos, std::shared_ptr<Texture> image,
		GLShaderProgram* shader) :
		shader(shader), image(image), pos(pos), size(image->getSize()),
		active(true), visible(true), children()
	{
	}

	UIElement::~UIElement()
	{
	}

	const mft::vec2i& UIElement::getPos() const
	{
		return pos;
	}

	const mft::vec2i& UIElement::getSize() const
	{
		return size;
	}

	void	UIElement::setPos(const mft::vec2i&& newPos)
	{
		this->pos = newPos;
	}

	void	UIElement::setSize(const mft::vec2i&& newSize)
	{
		this->size = newSize;
	}

	void	UIElement::draw() const
	{
		if (image != nullptr)
			image->draw(pos, size, 0.0f, mft::vec4(1.0f));
		for (auto& child : children)
		{
			child->draw();
		}
		for (const auto& text: this->texts)
		{
			if (text.font != nullptr)
			{
				text.font->RenderText(text.text, this->pos + text.pos, text.scale, text.color);
			}
		}
	}

	void	UIElement::update(const mft::vec2i& mousePos, const InputState mouseState)
	{
		for (auto& child : children)
		{
			child->update(mousePos, mouseState);
		}
	}

	constexpr void	UIElement::enable()
	{
		active = true;
	}

	constexpr void	UIElement::disable()
	{
		active = false;
	}

	constexpr void	UIElement::show()
	{
		visible = true;
	}

	constexpr void	UIElement::hide()
	{
		visible = false;
	}

}
