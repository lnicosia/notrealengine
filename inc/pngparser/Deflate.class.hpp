
#ifndef  __DEFLATE_CLASS_H__
# define __DEFLATE_CLASS_H__

# include "pngparser/BitStream.class.hpp"

# include <array>
# include <vector>
# include <memory>

namespace notrealengine
{
	class Deflate {
	private:
		const int SLIDING_WINDOW = 32768;
		const int EXTRA_BUFFER = 16384;

		class CodesBuffer {
		public:
			CodesBuffer( std::ostream & );
			~CodesBuffer();

			void copyCodes( const std::string & codes );
			void copyFromBuffer( unsigned int length, unsigned int distance );
			void copyCode( unsigned char codes );
			unsigned int getAlder32( void );

		private:
			void updateAlder32( unsigned int val );

			std::ostream & out;

			std::vector<unsigned char> buffer;
			unsigned int sum = 1;
			unsigned int sumsum = 0;
		};

		class HuffmanTree {
		public:
			HuffmanTree( const std::vector<unsigned char> & );
			~HuffmanTree();

			int getValue( unsigned int, unsigned char ) const;

		private:
			struct BinaryTree {
				int value = -1;

				std::unique_ptr<BinaryTree> bit0;
				std::unique_ptr<BinaryTree> bit1;

				void addValue( int, unsigned int, unsigned char );
				int getValue( unsigned int, unsigned char ) const;
			};

			BinaryTree tree;
		};

	public:

		static unsigned int parse( BitStream & in, std::ostream & out );
		static void parse_uncompressed_block( BitStream & in, CodesBuffer & out );
		static void parse_dynamic_huffman_codes( BitStream & in, CodesBuffer & out );
		static void parse_compressed_data( BitStream & in, const HuffmanTree & litterals, const HuffmanTree & distances, CodesBuffer & out );

		static std::vector<unsigned char> parse_lencodes( BitStream & in, unsigned int count );
		static std::vector<unsigned char> parse_huffmancodes( BitStream & in, unsigned int count, const HuffmanTree & lencodes );
	};
}

#endif

