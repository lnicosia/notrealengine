#ifndef _UIMANAGER_CLASS_H_
#define _UIMANAGER_CLASS_H_

#include "UI/UIElement.class.hpp"
#include <vector>

namespace notrealengine
{

	/**	Registers and handles all UI elements
	*/
	class UIManager
	{
	public:
		UIManager();
		~UIManager();

		/**	Update elements state according to the mouse's pos
		*/
		void
			update(const mft::vec2i& mousePos, const InputState mouseState);

		/**	Draw elements according to their state
		*/
		void
			draw( void );

		/**
		*/
		void
			registerElement(std::shared_ptr<UIElement> element);

		std::vector<std::shared_ptr<UIElement>>	elements;
	};

}
#endif // !_UIMANAGER_CLASS_H_
