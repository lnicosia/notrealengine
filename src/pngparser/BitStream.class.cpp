
#include "pngparser/BitStream.class.hpp"

#include <stdexcept>

namespace notrealengine
{
	BitStream::BitStream( void )
	{
		bitshift = 8;
		currentbyte = 0;
		stream.exceptions(std::stringstream::failbit | std::stringstream::badbit);
		return ;
	}

	BitStream::~BitStream( void )
	{

		return ;
	}

	bool BitStream::getBit()
	{
		if (bitshift >= 8)
		{
			currentbyte = stream.get();
			bitshift = 0;
		}
		bool ret = ((currentbyte >> bitshift) & 0x1) != 0;
		bitshift += 1;
		return ret;
	}

	void BitStream::discardLeftoverBits()
	{
		bitshift = 8;
	}

	unsigned int BitStream::getBits( unsigned int I )
	{
		unsigned int ret = 0;
		unsigned int shift = 0;

		if (I <= 0)
			return 0;
		if (I > 32)
			throw std::runtime_error("Trying to read more then 32 bits at once from bitstream!");

		ret |= (currentbyte >> bitshift) % (1 << I);
		shift = 8 - bitshift;
		bitshift += I;

		for (; I >= shift + 8; shift += 8)
		{
			currentbyte = stream.get();
			ret |= currentbyte << shift;
			bitshift = 8;
		}
		if (I > shift)
		{
			currentbyte = stream.get();
			ret |= ((currentbyte % (1 << (I - shift))) << shift);
			bitshift = I - shift;
		}
		return ret;
	}

	std::string BitStream::getBytes( unsigned int count )
	{
		if (bitshift >= 8)
			bitshift = 0;
		if (bitshift != 0)
			throw std::runtime_error("Trying to read bytes when not aligned with bytes boundary!");

		std::string bytes(count, '\0');

		stream.read(&bytes[0], count);

		return bytes;
	}
}
