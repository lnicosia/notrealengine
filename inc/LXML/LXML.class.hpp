#ifndef _LMXML_CLASS_H_
#define _LMXML_CLASS_H_

#include <string>
#include <map>
#include <vector>

namespace lxml
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
		std::vector<Tag>	children;

		bool valid;

	};

	class Importer
	{
	public:
		Importer();
		~Importer();

		void
			ReadFile(const std::string& path, unsigned int flags = 0);

		/**	Search for a tag in the hierarchy and returns it
		*	@return The expected Tag or null if not found
		*/
		const Tag*
			FindTag(const Tag& tag, const std::string& name) const;

		/**	Tags stored as a vector for reading utilies and optimization
		*/
		std::vector<Tag>	tags;

		/**	Tags stored as the original recursive hierarchy
		*/
		Tag RootTag;

	private:

		/**	Get the next word, i.e. string composed of alpha characters only
		**	and advance in the string just after it
		*/
		const char*
			GetNextWord(const char* str, std::string& word);

		/**	Get the next word, i.e. string composed of ascii characters
		**	surrounded by quotes
		**	and advance in the string just after it
		*/
		const char*
			GetNextString(const char* str, std::string& word);

		/*	Retrieve the first tag (between '<' and '>') found in the given string
		*/
		const char*
			GetNextTag(const char* str);

		/**	Retrieve the next attribute of a tag
		**	Return one of the possible attribute states:
		**	Valid, Invalid, Empty
		**	Set the attribute ptr given in args
		*/
		const char*
			GetNextAttribute(const char* str, int& success, Attribute& attribute);

		/**	Retrieve the next attribute of a tag
		**	Return one of the possible attribute states:
		**	Valid, Invalid, Empty
		**	Set the attribute ptr given in args
		**	Accepts ? at the end for the first line of an xml file
		*/
		const char*
			GetNextFirstLineAttribute(const char* str, int& success, Attribute& attribute);

		/** Read the name of a tag. Return the string after the name
		*/
		const char*
			ReadTagName(const char* str, std::string& name);

		/**
		*/
		bool
			CheckXml(const char* str);

		/**
		*/
		const char*
			ReadAttributes(const char* str, std::vector<Attribute>& attributes);

		/**
		*/
		const char*
			ReadFirstLineAttributes(const char* str, std::vector<Attribute>& attributes);

		/** Read a tag from content and create a Tag struct with
		*	its data
		*/
		const char*
			ReadTag(const char* str, Tag& tag);

		/**	Return the length of a tag, ie the number of characters
		**	until the first '>' or the end of the string
		*/
		size_t
			TagLen(const char* str);

	};

	/**	Retrieve an int from an Attribute vector
	**	Throws if overflow
	*/
	void
		GetIntAttribute(const Tag& tag, const std::string& name, int& nb);

	/**	Retrieve an unsigned int from an Attribute vector
	**	Throws if overflow
	*/
	void
		GetUIntAttribute(const Tag& tag, const std::string& name, unsigned int& nb);

	/**	Retrieve an str from an Attribute vector
	*/
	void
		GetStrAttribute(const Tag& tag, const std::string& name, std::string& str);

	/**	Returns if c is a tolerated whitespace in our current version
		*/
	bool
		IsValidWhitespace(char c);

	/**	Skip all the tolerated whitespaces at the beginning of the string
	*/
	const char*
		SkipWhitespaces(const char* str);

	std::ostream& operator<<(std::ostream& o, const Tag& tag);
	
}

#endif // !_LMXML_CLASS_H_
