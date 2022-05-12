#include "UI/UIElement.class.hpp"

namespace notrealengine
{
	UIElement::UIElement(GLShaderProgram* shader, std::shared_ptr<Texture> image,
		mft::vec2i pos) :
		shader(shader),
		image(image),
		pos(pos),
		active(true), visible(true),
		size(image->getSize()),
		children()
	{
	}

	UIElement::~UIElement()
	{
	}

	const mft::vec2i& UIElement::getPos() const
	{
		return pos;
	}

	void	UIElement::setPos(const mft::vec2i&& newPos)
	{
		pos = newPos;
	}

	void	UIElement::draw() const
	{
		image->draw(pos, size, 0.0f, mft::vec4(1.0f));
		for (auto& child : children)
		{
			child->draw();
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