
#ifndef  __BITSTREAM_CLASS_H__
# define __BITSTREAM_CLASS_H__

#include <iostream>
#include <sstream>

namespace notrealengine
{
	class BitStream {

	public:

		BitStream( void );
		~BitStream( void );

		bool getBit();
		void discardLeftoverBits();

		unsigned int getBits( unsigned int I );

		std::string getBytes( unsigned int );

		template<typename T>
		BitStream & operator<<( T & );

	private:
		std::stringstream stream;
		unsigned char currentbyte;
		unsigned int bitshift;
	};

	template<typename T>
	BitStream & BitStream::operator<<( T & in )
	{
		stream << in;
		return (*this);
	}
}

#endif

