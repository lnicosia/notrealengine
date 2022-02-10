#ifndef _COLLADA_PARSER_CLASS_H_
#define	_COLLADA_PARSER_CLASS_H_

#include <string>

namespace notrealengine
{
	class ColladaParser
	{
	public:
		ColladaParser();
		~ColladaParser();

		void
			ReadFile(const std::string& path, unsigned int flags);
	private:
	};


}

#endif // !_COLLADA_PARSER_CLASS_H_
