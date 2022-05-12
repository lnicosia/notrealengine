#include "UI/UIManager.class.hpp"
#include "GL.hpp"

bool	isMouseOnUI;

namespace notrealengine
{
	UIManager::UIManager(): elements()
	{
		isMouseOnUI = false;
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
		isMouseOnUI = true;
		//	Check if the mouse is inside any free zone
		//	It can still be on a button contained in a free zone. Buttons will
		//	update isMouseOnUI in this case
		for (const auto& freeZone: freeZones)
		{
			if (mousePos.x > freeZone.pos.x
				&& mousePos.x < freeZone.pos.x + freeZone.size.x
				&& mousePos.y > freeZone.pos.y
				&& mousePos.y < freeZone.pos.y + freeZone.size.y)
				isMouseOnUI = false;
		}

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
			element->draw();
		}
		GLCallThrow(glEnable, GL_DEPTH_TEST);
	}
}
