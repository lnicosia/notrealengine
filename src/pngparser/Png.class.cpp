
#include "pngparser/Png.class.hpp"
#include "pngparser/Deflate.class.hpp"

#include <stdexcept>
#include <utility>
#include <sstream>
#include <bitset>

namespace notrealengine
{
	Png::Png( std::istream & fstream )
	{
		try {
			parse(fstream);
		} catch (std::ifstream::failure & e) {
			std::stringstream tmp;
			tmp << "Error while reading PNG stream" << std::endl;
			tmp << e.what();
			throw png_exception(tmp.str());
		} catch (std::exception & e) {
			std::stringstream tmp;
			tmp << "Error while parsing PNG stream" << std::endl;
			tmp << e.what();
			throw png_exception(tmp.str());
		}
	}

	Png::~Png( void )
	{
	}

	void Png::parse( std::istream & fstream )
	{
		//fstream.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		check_signature(fstream);
		Chunk IHDR(fstream);
		if (IHDR.type.compare("IHDR") != 0)
			throw png_exception(std::string("PNG doesn't start with IHDR chunk"));
		EncodingData encoding = parse_IHDR_data(IHDR);
		BitStream zlib_stream;
		while (true)
		{
			Chunk newchunk(fstream);
			if (newchunk.type.compare("IEND") == 0)
				break ;
			else if (newchunk.type.compare("IDAT") == 0)
			{
				zlib_stream << newchunk.data;
			}
			else if ((newchunk.type[0] & 0x20) == 0)
				throw png_exception("PNG : Unsupported critical chunk '" + newchunk.type + "'");
		}
		std::stringstream filtered_lines;
		filtered_lines.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		parse_zlib(zlib_stream, filtered_lines);

		std::stringstream unfiltered_bytes;
		unfiltered_bytes.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		unfilter_lines(filtered_lines, unfiltered_bytes, encoding);

		generate_pixels(unfiltered_bytes, encoding);
	}

	void Png::check_signature( std::istream & fstream )
	{
		std::string bytes = parse_bytes(fstream, 8);

		//for (int i = 0; i < 8; i++)
		//{
		//	std::cout << "Byte[" << i << "] = " << (int)(bytes[i]) << std::endl;
		//}

		if (bytes.compare(std::string({(char)137, 80, 78, 71, 13, 10, 26, 10})) != 0)
			throw png_exception(std::string("Invalid PNG signature : ") + bytes);
	}

	Png::EncodingData Png::parse_IHDR_data( Chunk & IHDR )
	{
		EncodingData ret;
		std::stringstream data(IHDR.data);

		size = {(int)parse_uint(data, true), (int)parse_uint(data, true)};
		ret.bitDepth = data.get();
		ret.colorType = data.get();
		ret.compressionMethod = data.get();
		ret.filterMethod = data.get();
		ret.interlaceMethod = data.get();

		if (ret.bitDepth != 8)
		{
			std::stringstream tmp;
			tmp << "Invalid PNG bitDepth : " << ret.bitDepth;
			throw png_exception(tmp.str());
		}
		if (ret.colorType != 2 && ret.colorType != 6)
		{
			std::stringstream tmp;
			tmp << "Invalid PNG color type : " << ret.colorType;
			throw png_exception(tmp.str());
		}
		if (ret.compressionMethod != 0)
		{
			std::stringstream tmp;
			tmp << "Invalid PNG compression method : " << ret.compressionMethod;
			throw png_exception(tmp.str());
		}
		if (ret.filterMethod != 0)
		{
			std::stringstream tmp;
			tmp << "Invalid PNG filtering method : " << ret.filterMethod;
			throw png_exception(tmp.str());
		}
		if (ret.interlaceMethod != 0)
		{
			std::stringstream tmp;
			tmp << "Invalid PNG interlacing method : " << ret.interlaceMethod;
			throw png_exception(tmp.str());
		}

		return ret;
	}

	void Png::generate_pixels( std::istream & in, const EncodingData & encoding )
	{
		for (int i = 0; i < size.x * size.y; i++)
		{
			pixel_bytes.push_back(in.get());
			pixel_bytes.push_back(in.get());
			pixel_bytes.push_back(in.get());
			if (encoding.colorType >= 4)
				pixel_bytes.push_back(in.get());
			else
				pixel_bytes.push_back(255);
		}
	}

	void Png::unfilter_lines( std::istream & in, std::ostream & out, const EncodingData & encoding )
	{
		unsigned int sample_size = encoding.bitDepth;
		unsigned int sample_count = 3 + (encoding.colorType >= 4 ? 1 : 0);
		unsigned int pixel_size = sample_size * sample_count / 8;
		unsigned int scanline_size = size.x * pixel_size;

		std::vector<unsigned char> previous_line;
		for (int i = 0; i < scanline_size + pixel_size; i++)
			previous_line.push_back(0);

		for (int i = 0; i < size.y; i++)
		{
			std::vector<unsigned char> current_line;
			for (int j = 0; j < pixel_size; j++)
				current_line.push_back(0);

			unsigned int filter_method = in.get();
			for (int j = 0; j < scanline_size; j++)
			{
				switch (filter_method)
				{
					case 0:
						current_line.push_back(in.get());
						break;
					case 1:
						current_line.push_back(in.get() + current_line[j]);
						break;
					case 2:
						current_line.push_back(in.get() + previous_line[j + pixel_size]);
						break;
					case 3:
						current_line.push_back(in.get() + (current_line[j] + (int)previous_line[j + pixel_size]) / 2);
						break;
					case 4:
						current_line.push_back(in.get() + paeth_predictor(current_line[j], previous_line[j + pixel_size], previous_line[j]));
						break;
					default:
						throw png_exception("Unsupported filter method!");
				}
				out << current_line.back();
			}

			previous_line = current_line;
		}
	}

	unsigned char Png::paeth_predictor(int a, int b, int c)
	{
		int p = a + b - c,
			pa = std::abs(p - a),
			pb = std::abs(p - b),
			pc = std::abs(p - c);

		return (pa <= pb && pa <= pc ? a : (pb <= pc ? b : c));
	}

	void Png::parse_zlib( BitStream & in, std::ostream & out )
	{
		unsigned int cm = in.getBits(4);
		unsigned int cinfo = in.getBits(4);
		unsigned int fcheck = in.getBits(5);
		bool fdict = in.getBit();
		unsigned int flevel = in.getBits(2);

		if (((cinfo << 12) | (cm << 8) | (flevel << 6) | (fdict << 5) | fcheck) % 31 != 0)
			throw png_exception("Corrupted zlib header");
		if (cm != 8)
		{
			std::stringstream tmp;
			tmp << "Invalid zlib compression method : " << cm << ", expected deflate (8)";
			throw png_exception(tmp.str());
		}
		if (cinfo > 7)
		{
			std::stringstream tmp;
			tmp << "Invalid zlib deflate window size : " << (0x100 << cinfo) << ", max allowed is (32768)";
			throw png_exception(tmp.str());
		}
		if (fdict)
			throw png_exception("Preset dictionaried are unsupported");

		unsigned int alder32 = 0;
		unsigned int alder32checksum = 0;
		try {
			alder32 = Deflate::parse(in, out);
			in.discardLeftoverBits();
			alder32checksum =
				(in.getBits(8) << 24) +
				(in.getBits(8) << 16) +
				(in.getBits(8) << 8) +
				in.getBits(8);
		} catch (std::exception & e) {
			std::stringstream tmp;
			tmp << "Error while parsing deflate stream : " << std::endl << e.what();
			throw png_exception(tmp.str());
		}
		if (alder32 != alder32checksum)
			throw png_exception("Incorrect alder32 checksum for zlib stream!");
	}

	Png::Chunk::Chunk( std::istream & fstream )
	{
		unsigned int crc;

		try {
			length = parse_uint(fstream, true);
		} catch (std::istream::failure & e) {
			throw png_exception("Stream ends but chunk length expected!");
		}

		try {
			type = parse_bytes(fstream, 4);
		} catch (std::istream::failure & e) {
			throw png_exception("Stream ends but chunk type expected!");
		}

		try {
			data = parse_bytes(fstream, length);
		} catch (std::istream::failure & e) {
			std::stringstream ss;
			ss << "Stream ends before end of chunk! " << length << " bytes expected";
			throw png_exception(ss.str());
		}

		try {
			crc = parse_uint(fstream, true);
		} catch (std::istream::failure & e) {
			throw png_exception("Stream ends but crc expected!");
		}

		if (!check_crc(crc))
			throw png_exception("Cyclic redundancy check failed");
	}

	Png::Chunk::~Chunk()
	{
	}

	bool Png::Chunk::check_crc( unsigned int crc )
	{
		unsigned int computed = 0xFFFFFFFF;

		for (char & c : type)
			computed = crc_step(computed, c);
		for (char & c : data)
			computed = crc_step(computed, c);
		return (~crc == computed);
	}

	unsigned int Png::Chunk::crc_step( unsigned int crc, unsigned char c )
	{
		unsigned int tmp;

		tmp = (crc ^ c) & 0xFF;

		for (int i = 0; i < 8; i++)
		{
			if ((tmp & 0x1) != 0)
				tmp = 0xedb88320 ^ (tmp >> 1);
			else
				tmp = tmp >> 1;
		}

		return tmp ^ (crc >> 8);
	}

	unsigned int Png::parse_uint( std::istream & fstream, bool reverse )
	{
		std::string bytes = parse_bytes(fstream, 4);
		if (reverse)
			bytes = std::string({bytes[3], bytes[2], bytes[1], bytes[0]});

		unsigned int ret = *reinterpret_cast<unsigned int *>(bytes.data());

		return ret;
	}

	std::string Png::parse_bytes( std::istream & fstream, unsigned int length )
	{
		std::string bytes(length, '\0');

		fstream.read(&bytes[0], length);

		return bytes;
	}

	const mft::vec2i & Png::getSize()
	{
		return size;
	}

	const std::vector<unsigned char> & Png::getPixels()
	{
		return pixel_bytes;
	}
}
