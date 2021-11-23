#ifndef _BINDING_CLASS_HPP_
# define _BINDING_CLASS_HPP_

#include <string>
#include <vector>

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
		Binding(std::string name, uint8_t key1, uint8_t key2, bool editable);
		~Binding();

		//	Accessors

		std::string const	getName() const;
		uint8_t const	getKey1() const;
		uint8_t const	getKey2() const;
		InputState const	getState() const;

		void	setKey1(uint8_t key);
		void	setKey2(uint8_t key);
		void	setState(InputState state);

		//	Events
		//	Actions to perform in each event type
		std::vector<int	(*)()>	onRelease;
		std::vector<int	(*)()>	onPress;
		std::vector<int	(*)()>	whenReleased;
		std::vector<int	(*)()>	whenPressed;

		Binding& operator=(Binding const& ref);

	private:
		uint8_t	key1;
		uint8_t	key2;

		InputState	state;

		std::string const	name;

		bool const	editable = true;

	};
}

#endif