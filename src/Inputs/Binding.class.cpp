#include "Inputs/Binding.class.hpp"

namespace notrealengine
{
	Binding::Binding(uint8_t key1, uint8_t key2, bool editable)
		: key1(key1), key2(key2), editable(editable), state(InputState::NRE_RELEASED),
		onRelease(NULL), onPress(NULL), whenReleased(NULL), whenPressed(NULL)
	{

	}

	Binding::~Binding()
	{

	}

	//	Accessors

	uint8_t const	Binding::getKey1() const
	{
		return key1;
	}

	uint8_t const	Binding::getKey2() const
	{
		return key2;
	}

	InputState const	Binding::getState() const
	{
		return state;
	}

	//	Setters

	void	Binding::setKey1(uint8_t key)
	{
		this->key1 = key;
	}

	void	Binding::setKey2(uint8_t key)
	{
		this->key2 = key;
	}

	void	Binding::setState(InputState state)
	{
		this->state = state;
	}
}