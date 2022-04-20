#include "UI/UIManager.class.hpp"
#include "GL.hpp"

namespace notrealengine
{
	UIManager::UIManager()
	{
	}

	UIManager::~UIManager()
	{
	}

	void	UIManager::registerElement(std::shared_ptr<UIElement> element)
	{
		elements.push_back(element);
	}

	void	UIManager::update(const mft::vec2i& mousePos, const InputState mouseState)
	{
		for (auto& element : this->elements)
		{
			if (element->active == true)
				element->update(mousePos, mouseState);
		}
	}

	void	UIManager::draw( void )
	{
		GLCallThrow(glDisable, GL_DEPTH_TEST);
		for (const auto& element : this->elements)
		{
			if (element->visible == true)
				element->draw();
		}
		GLCallThrow(glEnable, GL_DEPTH_TEST);
	}
}
