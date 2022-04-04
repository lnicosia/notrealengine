#include "LXML/LXML.class.hpp"

#include <filesystem>
#include <iostream>
#include <fstream>
#include <ctype.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>

namespace lxml
{
	Importer::Importer()
	{
	}

	Importer::~Importer()
	{
	}

	void	Importer::ReadFile(const std::string& path, unsigned int flags)
	{
		std::filesystem::path	fPath(path);
		if (!std::filesystem::exists(fPath))
		{
			std::cerr << "lxml: Unable to open file \"" << path << "\"" << std::endl;
			return;
		}
		struct stat fileStats;
		lstat(path.c_str(), &fileStats);
		if (!S_ISREG(fileStats.st_mode))
		{
			std::cerr << "lxml: Invalid file type" << std::endl;
			return ;
		}
		std::ifstream file;
		std::string content;
		std::stringstream	stream;
		file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		try
		{
			file.open(path);
			stream << file.rdbuf();
			file.close();

			content = stream.str();
		}
		catch (std::ifstream::failure& e)
		{
			std::cerr << "lxml: Unable to open file \"" << path << "\"" << std::endl;
			return;
		}
		const char* str = content.c_str();
		const char* tag;
		try
		{
			tag = GetNextTag(str);
		}
		catch (std::exception& e)
		{
			std::cerr << "lxml: Invalid xml" << std::endl;
		}
		if (CheckXml(tag))
			return;
		try
		{
			str = ReadTag(str, this->RootTag);
		}
		catch (std::exception e)
		{
			std::cerr << "lxml: Invalid xml" << std::endl;
			return;
		}
		//std::cout << this->RootTag;
	}

	const char* Importer::GetNextTag(const char* str)
	{
		const char* skipped = SkipWhitespaces(str);
		size_t pos = 0;
		while (str[pos] && str[pos] != '<')
			pos++;
		return skipped + pos;
	}

	const char* Importer::GetNextString(const char* str, std::string& word)
	{
		str = SkipWhitespaces(str);
		if (TagLen(str) < 2 || str[0] != '"')
		{
			std::cerr << "Invalid string" << std::endl;
			throw std::exception();
		}
		size_t end = 1;
		while (str[end] && str[end] != '"' && isascii(str[end]))
			end++;
		if (str[end] != '"')
			throw std::exception();
		word = std::string(str + 1, end - 1);
		word.erase(std::remove_if(word.begin(), word.end(), ::isspace), word.end());
		return str + end;
	}

	const char* Importer::GetNextWord(const char* str, std::string& word)
	{
		str = SkipWhitespaces(str);
		size_t end = 0;
		while (str[end] && str[end] != '=' && (isalpha(str[end])
			|| str[end] == '_' || str[end] == ':'))
			end++;
		word = std::string(str, end);
		word.erase(std::remove_if(word.begin(), word.end(), ::isspace), word.end());
		return str + end;
	}

	const char*	Importer::GetNextAttribute(const char* str, int& success, Attribute& attribute)
	{
		str = GetNextWord(str, attribute.name);
		if (attribute.name == "" || attribute.name == ">")
		{
			success = AttributeState::Empty;
			return str;
		}
		str = SkipWhitespaces(str);
		if (!*str || str[0] != '=')
		{
			success = AttributeState::Invalid;
		}
		str++;
		str = SkipWhitespaces(str);
		if (!*str)
		{
			success = AttributeState::Invalid;
			return str;
		}
		str = GetNextString(str, attribute.value);
		if (!*str || str[0] != '"')
		{
			success = AttributeState::Invalid;
			return str;
		}
		success = AttributeState::Valid;
		str++;
		return str;
	}

	const char*	Importer::GetNextFirstLineAttribute(const char* str, int& success, Attribute& attribute)
	{
		str = GetNextWord(str, attribute.name);
		if (attribute.name == "" || attribute.name == "?>" || attribute.name == ">")
		{
			success = AttributeState::Empty;
			return str;
		}
		str = SkipWhitespaces(str);
		if (!*str || str[0] != '=')
		{
			success = AttributeState::Invalid;
			return str;
		}
		str++;
		str = SkipWhitespaces(str);
		if (!*str)
		{
			success = AttributeState::Invalid;
			return str;
		}
		str = GetNextString(str, attribute.value);
		if (!*str || str[0] != '"')
		{
			success = AttributeState::Invalid;
		}
		success = AttributeState::Valid;
		str++;
		return str;
	}

	const char* Importer::ReadAttributes(const char* str, std::vector<Attribute>& attributes)
	{
		while (*str)
		{
			Attribute newAttr;
			int attrState;
			str = GetNextAttribute(str, attrState, newAttr);
			if (attrState == AttributeState::Valid)
				attributes.push_back(newAttr);
			else if (attrState == AttributeState::Empty)
				break;
			else if (attrState == AttributeState::Invalid)
			{
				std::cerr << "lxml: Invalid xml attribute" << std::endl;
				std::cerr << "Line = " << std::endl << str << std::endl;
				std::cerr << "Attribute name = " << newAttr.name << std::endl;
				std::cerr << "Attribute value = " << newAttr.value << std::endl;
				throw std::exception();
			}
		}
		return str;
	}

	const char* Importer::ReadFirstLineAttributes(const char* str, std::vector<Attribute>& attributes)
	{
		while (*str)
		{
			Attribute newAttr;
			int attrState;
			str = GetNextAttribute(str, attrState, newAttr);
			if (attrState == AttributeState::Valid)
				attributes.push_back(newAttr);
			else if (attrState == AttributeState::Empty)
				break;
			else if (attrState == AttributeState::Invalid)
			{
				std::cerr << "lxml: Invalid xml first line attribute" << std::endl;
				std::cerr << "Line = " << std::endl << str << std::endl;
				std::cerr << "Attribute name = " << newAttr.name << std::endl;
				std::cerr << "Attribute value = " << newAttr.value << std::endl;
				throw std::exception();
			}
		}
		return str;
	}

	const char*	Importer::ReadTagName(const char* str, std::string& name)
	{
		size_t len = TagLen(str);
		if (len < 3 || str[0] != '<' || str[1] == '<'
			|| str[len - 1] != '>' || str[len - 2] == '>')
		{
			std::cerr << "Name error" << std::endl;
			std::cerr << "Len = " << len << std::endl;
			std::cerr << "str[0] = " << str[0] << std::endl;
			std::cerr << "str[1] = " << str[1] << std::endl;
			std::cerr << "str[len] = " << str[len] << std::endl;
			std::cerr << "str[len - 1] = " << str[len - 1] << std::endl;
			std::cerr << "str = " << str << std::endl;
			throw std::exception();
		}
		size_t pos = 1;
		while (str[pos] && !IsValidWhitespace(str[pos]) && str[pos] != '>')
			pos++;
		//std::string name = str.substr(1, pos - 1);
		name = std::string(str + 1, pos - 1);
		return str + pos;
	}

	bool	Importer::CheckXml(const char* str)
	{
		std::string name;
		try
		{
			str = ReadTagName(str, name);
		}
		catch (std::exception e)
		{
			return true;
		}
		if (name != "?xml")
		{
			std::cerr << "lxml: Invalid xml first line name (" << name << std::endl;
			std::cerr << "lxml: Invalid xml" << std::endl;
			return false;
		}
		std::vector<Attribute>	attributes;
		try
		{
			str = ReadFirstLineAttributes(str, attributes);
		}
		catch (std::exception e)
		{
			return true;
		}
		if (attributes.size() != 2
			|| attributes[0].name != "version"
			|| attributes[1].name != "encoding"
			|| attributes[1].value != "utf-8")
		{
			std::cerr << "lxml: Invalid xml first line data" << std::endl;
			std::cerr << "lxml: Invalid xml" << std::endl;
			return true;
		}
		size_t len = TagLen(str);
		if (str[len - 1] != '>' || str[len - 2] != '?')
		{
			std::cerr << "lxml: Invalid xml first line format" << std::endl;
			std::cerr << "lxml: Invalid xml" << std::endl;
			return true;
		}
		return false;
	}

	const char* Importer::ReadTag(const char* str, Tag& tag)
	{
		static int count = 0;
		tag.valid = false;
		if (TagLen(str) == 0)
			return str;
		std::string line;
		try
		{
			str = GetNextTag(str);
		}
		catch (std::exception& e)
		{
			std::cerr << "Get tag failure" << std::endl;
			throw e;
		}
		str = SkipWhitespaces(str);
		size_t len = TagLen(str);
		bool opening;
		if (str[1] != '/' && str[len - 2] != '/')
			opening = true;
		else
			opening = false;
		//std::cerr << "Current line = " << std::string(str, len + 1) << std::endl;
		if (len < 2 || str[0] != '<' || str[len - 1] != '>')
		{
			std::cerr << "lxml: Invalid tag line" << std::endl;
			throw std::exception();
		}
		std::string name;
		try
		{
			str = ReadTagName(str, name);
		}
		catch (std::exception e)
		{
			std::cerr << "lxml: Invalid name:" << e.what() << std::endl;
			throw e;
		}
		//std::cerr << "Name = " << name << std::endl;
		std::vector<Attribute>	attributes;
		try
		{
			str = ReadAttributes(str, attributes);
		}
		catch (std::exception e)
		{
			std::cerr << "lxml: Invalid attributes" << std::endl;
			throw e;
		}
		tag.name = name;
		tag.attributes = attributes;
		str++;
		str = SkipWhitespaces(str);
		if (strlen(str) != 0 && str[0] != '<')
		{
			size_t pos = 0;
			while (str[pos] && str[pos] != '<')
				pos++;
			if (pos == strlen(str))
			{
				std::cerr << "Invalid content" << std::endl;
				std::cerr << "Content = " << str << std::endl;
				throw std::exception();
			}
			tag.content = std::string(str, pos);
			str += pos;
		}
		tag.valid = true;
		//std::cerr << *newTag << std::endl;
		if (opening == true)
		{
			bool isClosed = false;
			while (isClosed == false)
			{
				Tag childTag;
				try
				{
					str = ReadTag(str, childTag);

				}
				catch (std::exception e)
				{
					std::cerr << "lxml: Invalid child" << std::endl;
					throw e;
				}
				if (childTag.valid == false)
				{
					isClosed = true;
				}
				else
				{

					if (childTag.name.size() >= 1
						&& childTag.name[0] == '/')
					{
						if (childTag.name.substr(1) != tag.name)
						{
							std::cerr << "Invalid closing tag: " << childTag.name;
							std::cerr << ". Expected: " << tag.name << std::endl;
							throw std::exception();
						}
						else
						{
							isClosed = true;
						}
					}
					else
						tag.children.push_back(childTag);
				}
			}
		}
		return str;
	}

	const Tag*	Importer::FindTag(const Tag& tag, const std::string& name) const
	{
		if (tag.valid == false)
			return nullptr;
		if (tag.name == name)
			return &tag;
		for (const auto& child : tag.children)
		{
			const Tag* res;
			res = FindTag(child, name);
			if (res != nullptr)
				return res;
		}
		return nullptr;
	}

	size_t	Importer::TagLen(const char* str)
	{
		size_t len = 0;
		while (str[len] && str[len] != '>')
			len++;
		if (str[len] == '>')
			len++;
		return len;
	}

	void	GetIntAttribute(const Tag& tag, const std::string& name, int& nb)
	{
		for (const auto& attr : tag.attributes)
		{
			if (attr.name == name)
			{
				try
				{
					nb = std::stoi(attr.value);
				}
				catch (std::out_of_range& e)
				{
					throw e;
				}
			}
		}
	}

	void	GetUIntAttribute(const Tag& tag, const std::string& name, unsigned int& nb)
	{
		for (const auto& attr : tag.attributes)
		{
			if (attr.name == name)
			{
				try
				{
					nb = static_cast<unsigned int>(std::stoul(attr.value));
				}
				catch (std::out_of_range& e)
				{
					throw e;
				}
			}
		}
	}

	void	GetStrAttribute(const Tag& tag, const std::string& name, std::string& str)
	{
		for (const auto& attr : tag.attributes)
		{
			if (attr.name == name)
			{
				str = attr.value;
			}
		}
	}

	bool	IsValidWhitespace(char c)
	{
		if (c == 0)
			return 0;
		if (c == ' ' || c == '\n' || c == '\t' || c == '\r' || c == '\f' || c == '\v')
			return true;
		return false;
	}

	const char* SkipWhitespaces(const char* str)
	{
		size_t pos = 0;
		while (str[pos] && IsValidWhitespace(str[pos]))
			pos++;
		return str + pos;
	}

	std::ostream& operator<<(std::ostream& o, const Tag& tag)
	{
		static int count = 0;
		for (int i = 0; i < count; i++)
			std::cout << "  ";
		count++;
		std::cout << "<" << tag.name;
		for (const Attribute& attribute : tag.attributes)
		{
			std::cout << " " << attribute.name;
			std::cout << "=\"" << attribute.value << "\"";
		}
		if (tag.children.size() == 0 && tag.content == "")
			std::cout << "/";
		std::cout << ">" << tag.content;
		bool first = true;
		for (auto& child : tag.children)
		{
			if (first == true)
			{
				std::cout << std::endl;
				first = false;
			}
			std::cout << child;
		}
		count--;
		if (tag.children.size() == 0 && tag.content == "")
		{
			std::cout << std::endl;
			return o;
		}
		if (tag.content == "")
		{
			for (int i = 0; i < count; i++)
				std::cout << "  ";
		}
		std::cout << "</" << tag.name << ">" << std::endl;
		return o;
	}
}
