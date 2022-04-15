#ifndef _BINDING_CLASS_H_
#define _BINDING_CLASS_H_

#include "Inputs/Action.class.hpp"

#include <string>
#include <vector>

enum class InputState
{
	NRE_PRESS,
	NRE_RELEASE,
	NRE_PRESSED,
	NRE_RELEASED,
	NRE_HOVERED
};

namespace notrealengine
{

	class Binding
	{
	public:
		Binding(std::string name, uint32_t key1, uint32_t key2, bool editable);
		~Binding();

		//	Accessors

		std::string const	getName() const;
		uint32_t const	getKey1() const;
		uint32_t const	getKey2() const;
		InputState const	getState() const;

		void	setKey1(uint32_t key);
		void	setKey2(uint32_t key);
		void	setState(InputState state);

		//	Events
		//	Actions to perform in each event type
		ActionWrapper*	onRelease;
		ActionWrapper*	onPress;
		ActionWrapper*	whenReleased;
		ActionWrapper*	whenPressed;

		Binding& operator=(Binding const& ref);

	private:
		uint8_t	key1;
		uint8_t	key2;

		InputState	state;

		std::string const	name;

		bool const	editable = true;

	};

}

#endif // !_BINDINGS_CLASS_H_
