
#ifndef  __PNG_CLASS_H__
# define __PNG_CLASS_H__

#include "mft/mft.hpp"

#include "pngparser/BitStream.class.hpp"

#include <filesystem>
#include <fstream>
#include <vector>

namespace notrealengine
{
	class Png {

	public:

		Png( std::filesystem::path file );
		~Png( void );

		const mft::vec2i &
			getSize();
		const std::vector<unsigned char> &
			getPixels();

	private:

		mft::vec2i size;
		std::vector<unsigned char> pixel_bytes;

		struct Chunk {

			Chunk( std::istream & );
			~Chunk( void );

			unsigned int length;
			std::string type;
			std::string data;

		private:
			bool check_crc( unsigned int );
			unsigned int static crc_step( unsigned int, unsigned char );
		};

		struct EncodingData
		{
			unsigned int bitDepth;
			unsigned int colorType;
			unsigned int compressionMethod;
			unsigned int filterMethod;
			unsigned int interlaceMethod;
		};

		void parse( std::istream & );
		void check_signature( std::istream & );
		EncodingData parse_IHDR_data( Chunk & );
		void parse_zlib( BitStream &, std::ostream & );
		void unfilter_lines( std::istream &, std::ostream &, const EncodingData & );
		void generate_pixels( std::istream &, const EncodingData & );

		unsigned char paeth_predictor(unsigned char, unsigned char, unsigned char);

		unsigned int static parse_uint( std::istream &, bool );
		std::string static parse_bytes( std::istream &, unsigned int );
	};

	class png_exception : public std::runtime_error {
	public:
		png_exception( std::string what_arg ): std::runtime_error(what_arg) {};
		~png_exception( void ) {};
	};
}

#endif

