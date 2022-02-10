#ifndef _LMXML_CLASS_H_
#define _LMXML_CLASS_H_

#include <string>
#include <map>
#include <vector>

namespace LXML
{
	enum AttributeState
	{
		Valid,
		Invalid,
		Empty
	};
	struct Attribute
	{
		std::string	name;
		std::string	value;
	};
	struct Tag
	{
		std::string id;
		std::string name;
		std::vector<Attribute> attributes;
		std::string	content;
		std::vector<Tag*>	children;

	};

	class Importer
	{
	public:
		Importer();
		~Importer();

		void
			ReadFile(const std::string& path, unsigned int flags = 0);
	private:
		/**	Returns if c is a tolerated whitespace in our current version
		*/
		bool
			IsValidWhitespace(char c);

		/**	Skip all the tolerated whitespaces at the beginning of the string
		*/
		void
			SkipWhitespaces(std::string& str);

		/**	Get the next word, i.e. string composed of alpha characters only
		**	and advance in the string just after it
		*/
		std::string
			GetNextWord(std::string& line);

		/**	Get the next word, i.e. string composed of ascii characters
		**	surrounded by quotes
		**	and advance in the string just after it
		*/
		std::string
			GetNextString(std::string& line);

		/*	Retrieve the first tag (between '<' and '>') found in the given string
		*/
		std::string
			GetTag(std::string& content);

		/**	Retrieve the next attribute of a tag
		**	Return one of the possible attribute states:
		**	Valid, Invalid, Empty
		**	Set the attribute ptr given in args
		*/
		int
			GetNextAttribute(std::string& line, Attribute& attribute);

		/**	Retrieve the next attribute of a tag
		**	Return one of the possible attribute states:
		**	Valid, Invalid, Empty
		**	Set the attribute ptr given in args
		**	Accepts ? at the end for the first line of an xml file
		*/
		int
			GetNextFirstLineAttribute(std::string& line, Attribute& attribute);

		/**
		*/
		int
			ReadTagName(std::string& line, std::string& name);

		/**
		*/
		bool
			CheckXml(std::string& content);

		/**
		*/
		std::vector<Attribute>
			ReadAttributes(const std::string& line);

		/**
		*/
		std::vector<Attribute>
			ReadFirstLineAttributes(const std::string& line);

		/**
		*/
		Tag*
			ReadTag(std::string& content);

		/**	Tags stored as a vector for reading utilies and optimization
		*/
		std::vector<Tag*>	tags;

		/**	Tags stored as the original recursive hierarchy 
		*/
		Tag* tag;
	};

	std::ostream& operator<<(std::ostream& o, const Tag& tag);
	
}

#endif // !_LMXML_CLASS_H_
