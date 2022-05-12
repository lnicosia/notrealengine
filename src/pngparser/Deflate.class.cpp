
#include "pngparser/Deflate.class.hpp"

#include <stdexcept>
#include <iostream>
#include <iomanip>
#include <bitset>

namespace notrealengine
{
	unsigned int Deflate::parse( BitStream & in, std::ostream & out )
	{
		CodesBuffer out_codes(out);
		bool bFinal = false;

		while (!bFinal)
		{
			bFinal = (in.getBit() == 1);
			unsigned int bType = in.getBits(2);

			if (bType == 0)
				parse_uncompressed_block( in, out_codes );
			//else if (bType == 1)
			//	parse_uncompressed_block( in, out_codes );
			else if (bType == 2)
				parse_dynamic_huffman_codes( in, out_codes );
			else
			{
				std::stringstream tmp;
				tmp << "Unknown deflate block type : " << bType;
				throw std::runtime_error(tmp.str());
			}
		}

		return out_codes.getAlder32();
	}

	void Deflate::parse_uncompressed_block( BitStream & in, CodesBuffer & out )
	{
		in.discardLeftoverBits();
		unsigned int len = in.getBits(16);
		unsigned int nlen = in.getBits(16);

		if (~len != nlen)
			throw std::runtime_error("Deflate block length corrupted!");
		std::string bytes = in.getBytes(len);

		out.copyCodes(bytes);
	}

	void Deflate::parse_dynamic_huffman_codes( BitStream & in, CodesBuffer & out )
	{
		unsigned int hlit = 257 + in.getBits(5);
		unsigned int hdist = 1 + in.getBits(5);
		unsigned int hlen = 4 + in.getBits(4);

		auto codelens = parse_lencodes(in, hlen);
		HuffmanTree lentree(codelens);

		auto codelitterals = parse_huffmancodes(in, hlit, lentree);
		HuffmanTree litteralstree(codelitterals);

		auto codedistances = parse_huffmancodes(in, hdist, lentree);
		HuffmanTree distancestree(codedistances);

		parse_compressed_data(in, litteralstree, distancestree, out);
	}

	void Deflate::parse_compressed_data( BitStream & in, const HuffmanTree & litterals, const HuffmanTree & distances, CodesBuffer & out )
	{
		const std::vector<unsigned int> lenbits{
			0, 0, 0, 0, 0, 0, 0, 0,
			1, 1, 1, 1,
			2, 2, 2, 2,
			3, 3, 3, 3,
			4, 4, 4, 4,
			5, 5, 5, 5,
			0
		};
		const std::vector<unsigned int> distbits{
			0, 0, 0, 0,
			1, 1, 2, 2,
			3, 3, 4, 4,
			5, 5, 6, 6,
			7, 7, 8, 8,
			9, 9, 10, 10,
			11, 11, 12, 12,
			13, 13
		};

		while (true)
		{
			unsigned char len = 0;
			unsigned int code = 0;

			while (litterals.getValue(code, len) < 0)
			{
				code = (code << 1) | in.getBits(1);
				len++;
			}

			int value = litterals.getValue(code, len);
			if (value <= 255)
			{
				out.copyCode((unsigned char)value);
			}
			else if (value == 256)
				break ;
			else
			{
				unsigned int length = 3;
				if (value == 285)
					length = 258;
				else for (unsigned int i = 0; i < value - 257; i++)
						length += (1 << lenbits[i]);
				if (lenbits[value - 257] > 0)
				{
					unsigned int bits = in.getBits(lenbits[value - 257]);
					length += bits;
				}

				len = 0;
				code = 0;
				while (distances.getValue(code, len) < 0)
				{
					code = (code << 1) | in.getBits(1);
					len++;
				}
				int distancecode = distances.getValue(code, len);
				unsigned int distance = 1;
				for (unsigned int i = 0; i < distancecode; i++)
					distance += (1 << distbits[i]);
				if (distbits[distancecode] > 0)
					distance += in.getBits(distbits[distancecode]);

				out.copyFromBuffer(length, distance);
			}
		}
	}

	std::vector<unsigned char> Deflate::parse_lencodes( BitStream & in, unsigned int count )
	{
		std::vector<unsigned char> ret{
			0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0
		};
		const std::vector<unsigned int> order{
			16, 17, 18, 0,
			8, 7, 9, 6, 10, 5, 11, 4, 12, 3, 13, 2, 14, 1, 15
		};

		for (int i = 0; i < count; i++)
			ret[order[i]] = in.getBits(3);
		return ret;
	}

	std::vector<unsigned char> Deflate::parse_huffmancodes( BitStream & in, unsigned int count, const HuffmanTree & lencodes )
	{
		std::vector<unsigned char> ret;
		unsigned int previous_len = 0;

		for (int i = 0; i < count; )
		{
			unsigned char len = 0;
			unsigned int code = 0;

			while (lencodes.getValue(code, len) < 0)
			{
				code = (code << 1) | in.getBits(1);
				len++;
			}
			int value = lencodes.getValue(code, len);
			if (value <= 15)
			{
				ret.push_back(value);
				previous_len = value;
				i++;
			}
			else if (value == 16)
			{
				unsigned char count = 3 + in.getBits(2);
				for (int j = 0; j < count; j++)
					ret.push_back(previous_len);
				i += count;
			}
			else if (value == 17)
			{
				unsigned char count = 3 + in.getBits(3);
				for (int j = 0; j < count; j++)
					ret.push_back(0);
				previous_len = 0;
				i += count;
			}
			else if (value == 18)
			{
				unsigned char count = 11 + in.getBits(7);
				for (int j = 0; j < count; j++)
					ret.push_back(0);
				previous_len = 0;
				i += count;
			}
			else
			{
				std::stringstream errstr;
				errstr << "Invalid length code in huffman tree" << value;
				throw std::runtime_error(errstr.str());
			}
		}
		return ret;
	}

	Deflate::HuffmanTree::HuffmanTree( const std::vector<unsigned char> & lengths )
	{
		int code = 0;
		bool maxlen = false;
		for (unsigned char len = 1; !maxlen; len++)
		{
			code <<= 1;
			maxlen = true;

			for (int i = 0; i < lengths.size(); i++)
			{
				if (lengths[i] == len)
				{
					tree.addValue(i, code, len);
					code++;
				}
				else if (lengths[i] > len)
					maxlen = false;
			}
		}
	}

	Deflate::HuffmanTree::~HuffmanTree( )
	{
	}

	int Deflate::HuffmanTree::getValue( unsigned int code, unsigned char length ) const
	{
		return tree.getValue(code, length);
	}

	void Deflate::HuffmanTree::BinaryTree::addValue( int _value, unsigned int code, unsigned char len )
	{
		if (len <= 0)
			value = _value;
		else
		{
			unsigned int bit = (code >> (len - 1)) & 1;
			std::unique_ptr<BinaryTree> & subtree(bit == 0 ? bit0 : bit1);
			if (subtree == nullptr)
				subtree = std::make_unique<BinaryTree>();
			subtree->addValue(_value, code, len - 1);
		}
	}

	int Deflate::HuffmanTree::BinaryTree::getValue( unsigned int code, unsigned char len ) const
	{
		if (len <= 0)
			return value;
		else
		{
			unsigned int bit = (code >> (len - 1)) & 1;
			const std::unique_ptr<BinaryTree> & subtree(bit == 0 ? bit0 : bit1);
			if (subtree == nullptr)
			{
				std::stringstream codebits;
				codebits << std::bitset<16>(code);
				std::stringstream errstr;
				errstr << "Invalid huffman tree code: " << codebits.str().substr(codebits.str().size() - len, len);

				throw std::runtime_error(errstr.str());
			}
			return subtree->getValue(code, len - 1);
		}
	}

	Deflate::CodesBuffer::CodesBuffer( std::ostream & out ):
		out(out)
	{
	}

	Deflate::CodesBuffer::~CodesBuffer()
	{
	}

	void Deflate::CodesBuffer::copyCodes( const std::string & codes )
	{
		out << codes;
		for (unsigned char c : codes)
		{
			buffer.push_back(c);
			updateAlder32(c);
		}
	}

	void Deflate::CodesBuffer::copyCode( unsigned char code )
	{
		out << code;
		buffer.push_back(code);
		updateAlder32(code);
	}

	void Deflate::CodesBuffer::copyFromBuffer( unsigned int length, unsigned int distance )
	{
		for (int i = 0; i < length; i++)
		{
			try {
				unsigned char c = buffer.at(buffer.size() - distance);
				copyCode(c);
			} catch (std::out_of_range) {
				std::stringstream strs;
				strs << "Distance '" << distance << "' is too big for buffer of size '" << buffer.size() << "'" << std::endl;
				throw std::runtime_error(strs.str());
			}
		}
	}

	void Deflate::CodesBuffer::updateAlder32( unsigned int val )
	{
		sum = (sum + val) % 65521;
		sumsum = (sumsum + sum) % 65521;
	}

	unsigned int Deflate::CodesBuffer::getAlder32( void )
	{
		return (sumsum << 16) | sum;
	}
}
