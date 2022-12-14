#include "UI/UIElement.class.hpp"

namespace notrealengine
{
	UIElement::UIElement(mft::vec2i pos, std::shared_ptr<Texture> image,
		GLShaderProgram* shader) :
		shader(shader), image(image), pos(pos), size(),
		active(true), visible(true), children()
	{
		if (image)
			size = image->getSize();
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

	std::shared_ptr<UIElement>	UIElement::getChild(size_t index)
	{
		if (index >= this->children.size())
			return nullptr;
		return this->children[index];
	}

	const std::vector<std::shared_ptr<UIElement>>&	UIElement::getChildren()
	{
		return this->children;
	}

	void	UIElement::setPos(const mft::vec2i&& newPos)
	{
		this->pos = newPos;
	}

	void	UIElement::setImage(std::shared_ptr<Texture> image)
	{
		this->image = image;
	}

	void	UIElement::addPos(const mft::vec2i& pos)
	{
		this->pos += pos;
		for (auto child : this->children)
		{
			child->addPos(pos);
		}
	}

	void	UIElement::setSize(const mft::vec2i&& newSize)
	{
		this->size = newSize;
	}

	void	UIElement::draw() const
	{
		if (image != nullptr && this->visible == true)
			image->draw(pos, size, 0.0f, mft::vec4(1.0f));
		for (auto& child : children)
		{
			child->draw();
		}
		for (const auto& text: this->texts)
		{
			if (text.font != nullptr)
			{
				//std::cout << "Writing " << text.text << " at pos " << text.pos + this->pos << std::endl;
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

	void UIElement::addChild(std::shared_ptr<UIElement> child)
	{
		child->addPos(this->pos);
		this->children.push_back(child);
	}

	void UIElement::clearChildren()
	{
		this->children.clear();
	}

}
