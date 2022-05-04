#ifndef _UIMANAGER_CLASS_H_
#define _UIMANAGER_CLASS_H_

#include "UI/UIElement.class.hpp"
#include <vector>

namespace notrealengine
{

	struct Rectangle
	{
		mft::vec2i pos;
		mft::vec2i size;
		Rectangle(mft::vec2i pos, mft::vec2i size): pos(pos), size(size)
		{}
	};
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

		/**	Register an element to be handled by the manager
		*/
		void
			registerElement(std::shared_ptr<UIElement> element);

		std::vector<std::shared_ptr<UIElement>>	elements;

		/**	Zones where the mouse is not on top of any UI.
		**	Except for potential buttons that will check it themselves
		*/
		std::vector<Rectangle> freeZones;
	};

}
#endif // !_UIMANAGER_CLASS_H_
