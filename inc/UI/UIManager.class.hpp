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

		/**	Draw and update elements according to their state
		*/
		void
			update(const mft::vec2i& mousePos, const InputState mouseState);

		/**
		*/
		void
			registerElement(std::shared_ptr<UIElement> element);

	private:
		std::vector<std::shared_ptr<UIElement>>	elements;
	};

}
#endif // !_UIMANAGER_CLASS_H_
