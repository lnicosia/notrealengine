#ifndef _BINDING_CLASS_HPP_
# define _BINDING_CLASS_HPP_

#include <string>

enum class InputState
{
	NRE_PRESS,
	NRE_RELEASE,
	NRE_PRESSED,
	NRE_RELEASED
};

namespace notrealengine
{

	class Binding
	{
	public:
		Binding(uint8_t key1, uint8_t key2, bool editable);
		~Binding();

		//	Accessors

		uint8_t const	getKey1() const;
		uint8_t const	getKey2() const;
		InputState const	getState() const;

		void	setKey1(uint8_t key);
		void	setKey2(uint8_t key);
		void	setState(InputState state);

		//	Events
		//	Actions to perform in each event type
		int		(*onRelease)(void);
		int		(*onPress)(void);
		int		(*whenReleased)(void);
		int		(*whenPressed)(void);

	private:
		uint8_t	key1;
		uint8_t	key2;

		InputState	state;

		std::string const	name;

		bool const	editable = true;

	};
}

#endif