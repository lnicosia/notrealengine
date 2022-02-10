#include "LXML/LXML.class.hpp"

#include <filesystem>
#include <iostream>
#include <fstream>
#include <ctype.h>

namespace LXML
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
		std::string tag;
		try
		{
			tag = GetTag(content);
		}
		catch (std::exception& e)
		{
			std::cerr << "lxml: Invalid xml" << std::endl;
		}
		if (CheckXml(tag))
			return;
		try
		{
			this->tag = ReadTag(content);
		}
		catch (std::exception e)
		{
			std::cerr << "lxml: Invalid xml" << std::endl;
			return;
		}
		//std::cout << *(this->tag);
	}

	std::string	Importer::GetTag(std::string& content)
	{
		SkipWhitespaces(content);
		if (content.size() < 2 || content[0] != '<')
		{
			std::cerr << "Invalid tag" << std::endl;
			throw std::exception();
		}
		std::string ret;
		size_t end = content.find_first_of('>');
		if (end == std::string::npos)
		{
			std::cerr << "No end of tag " << std::endl;
			throw std::exception();
			return ret;
		}
		end++;
		if (end >= content.size())
			ret = content;
		else
			ret = (content.substr(0, end));
		//end++;
		if (end >= content.size())
			content.clear();
		else	
			content = content.substr(end);
		return ret;
	}

	std::string	Importer::GetNextString(std::string& line)
	{
		SkipWhitespaces(line);
		if (line.size() < 2 || line[0] != '"')
		{
			std::cerr << "Invalid string" << std::endl;
			throw std::exception();
		}
		size_t end = 1;
		while (line[end] && line[end] != '"' && isascii(line[end]))
			end++;
		if (line[end] != '"')
			throw std::exception();
		std::string word = line.substr(1, end - 1);
		word.erase(std::remove_if(word.begin(), word.end(), ::isspace), word.end());
		line = line.substr(end);
		return word;
	}

	std::string	Importer::GetNextWord(std::string& line)
	{
		SkipWhitespaces(line);
		size_t end = 0;
		while (line[end] && line[end] != '=' && (isalpha(line[end])
			|| line[end] == '_' || line[end] == ':'))
			end++;
		std::string word = line.substr(0, end);
		word.erase(std::remove_if(word.begin(), word.end(), ::isspace), word.end());
		line = line.substr(end);
		return word;
	}

	int	Importer::GetNextAttribute(std::string& line, Attribute& attribute)
	{
		attribute.name = GetNextWord(line);
		if (attribute.name == "" || attribute.name == ">")
			return AttributeState::Empty;
		SkipWhitespaces(line);
		if (line.empty() || line[0] != '=')
			return AttributeState::Invalid;
		line = line.substr(1);
		SkipWhitespaces(line);
		if (line.empty())
			return AttributeState::Invalid;
		attribute.value = GetNextString(line);
		if (line.empty() || line[0] != '"')
			return AttributeState::Invalid;
		line = line.substr(1);
		return AttributeState::Valid;
	}

	int	Importer::GetNextFirstLineAttribute(std::string& line, Attribute& attribute)
	{
		attribute.name = GetNextWord(line);
		if (attribute.name == "" || attribute.name == "?>" || attribute.name == ">")
			return AttributeState::Empty;
		SkipWhitespaces(line);
		if (line.empty() || line[0] != '=')
			return AttributeState::Invalid;
		line = line.substr(1);
		SkipWhitespaces(line);
		if (line.empty())
			return AttributeState::Invalid;
		attribute.value = GetNextString(line);
		if (line.empty() || line[0] != '"')
			return AttributeState::Invalid;
		line = line.substr(1);
		return AttributeState::Valid;
	}

	bool	Importer::IsValidWhitespace(char c)
	{
		if (c == ' ' || c == '\n' || c == '\t' || c == '\r' || c == '\f' || c == '\v')
			return true;
		return false;
	}

	void	Importer::SkipWhitespaces(std::string& str)
	{
		size_t pos = 0;
		while (str[pos] && IsValidWhitespace(str[pos]))
			pos++;
		str = str.substr(pos);
	}

	std::vector<Attribute> Importer::ReadAttributes(const std::string& line)
	{
		std::vector<Attribute>	attributes;
		std::string str = line;
		while (str.empty() == false)
		{
			Attribute newAttr;
			int attrState = GetNextAttribute(str, newAttr);
			if (attrState == AttributeState::Valid)
				attributes.push_back(newAttr);
			else if (attrState == AttributeState::Empty)
				break;
			else if (attrState == AttributeState::Invalid)
			{
				std::cerr << "lxml: Invalid xml attribute" << std::endl;
				std::cerr << "Line = " << std::endl << line << std::endl;
				std::cerr << "Attribute name = " << newAttr.name << std::endl;
				std::cerr << "Attribute value = " << newAttr.value << std::endl;
				throw std::exception();
				return attributes;
			}
		}
		return attributes;
	}

	std::vector<Attribute> Importer::ReadFirstLineAttributes(const std::string& line)
	{
		std::vector<Attribute>	attributes;
		std::string str = line;
		while (str.empty() == false)
		{
			Attribute newAttr;
			int attrState = GetNextFirstLineAttribute(str, newAttr);
			if (attrState == AttributeState::Valid)
				attributes.push_back(newAttr);
			else if (attrState == AttributeState::Empty)
				break;
			else if (attrState == AttributeState::Invalid)
			{
				std::cerr << "lxml: Invalid xml first line attribute" << std::endl;
				std::cerr << "Line = " << std::endl << line << std::endl;
				std::cerr << "Attribute name = " << newAttr.name << std::endl;
				std::cerr << "Attribute value = " << newAttr.value << std::endl;
				throw std::exception();
				return attributes;
			}
		}
		return attributes;
	}
	
	int	Importer::ReadTagName(std::string& line, std::string& name)
	{
		if (line.size() < 3 || line[0] != '<' || line[1] == '<'
			|| line[line.size() - 1] != '>' || line[line.size() - 2] == '>')
		{
			std::cerr << "Name error" << std::endl;
			throw std::exception();
		}
		size_t pos = 1;
		while (line[pos] && !IsValidWhitespace(line[pos]) && line[pos] != '>')
			pos++;
		name = line.substr(1, pos - 1);
		line = line.substr(pos);
		return 0;
	}

	bool	Importer::CheckXml(std::string& line)
	{
		std::string name;
		try
		{
			ReadTagName(line, name);
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
			attributes = ReadFirstLineAttributes(line);
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
		if (line[line.size() - 1] != '>' || line[line.size() - 2] != '?')
		{
			std::cerr << "lxml: Invalid xml first line format" << std::endl;
			std::cerr << "lxml: Invalid xml" << std::endl;
			return true;
		}
		return false;
	}

	Tag* Importer::ReadTag(std::string& content)
	{
		if (content.empty())
			return nullptr;
		static int count = 0;
		Tag* newTag = nullptr;
		std::string line;
		try
		{
			line = GetTag(content);
		}
		catch (std::exception& e)
		{
			std::cerr << "Get tag failure" << std::endl;
			throw e;
			return nullptr;
		}
		size_t spaces = 0;
		while (line[spaces] == ' ' || line[spaces] == '\t')
			spaces++;
		line = line.substr(spaces);
		bool opening;
		if (line[1] != '/' && line[line.size() - 2] != '/')
			opening = true;
		else
			opening = false;
		//std::cerr << "Current line = " << line << std::endl;
		if (line.size() < 2 || line[0] != '<' || line[line.size() -1] != '>')
		{
			std::cerr << "lxml: Invalid tag line" << std::endl;
			throw std::exception();
		}
		std::string name;
		try
		{
			ReadTagName(line, name);
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
			attributes = ReadAttributes(line);
		}
		catch (std::exception e)
		{
			std::cerr << "lxml: Invalid attributes" << std::endl;
			throw e;
		}
		newTag = new Tag;
		newTag->name = name;
		newTag->attributes = attributes;
		SkipWhitespaces(content);
		if (!content.empty() && content[0] != '<')
		{
			size_t pos = content.find_first_of('<');
			if (pos == std::string::npos)
				throw std::exception();
			newTag->content = content.substr(0, pos);
			content = content.substr(pos);
		}
		//std::cerr << *newTag << std::endl;
		if (opening == true)
		{
			bool isClosed = false;
			while (isClosed == false)
			{
				Tag* childTag;
				try
				{
					childTag = ReadTag(content);

				}
				catch (std::exception e)
				{
					std::cerr << "lxml: Invalid child" << std::endl;
					throw e;
				}
				if (childTag == nullptr)
				{
					isClosed = true;
				}
				else
				{
					
					if (childTag->name.size() >= 1
						&& childTag->name[0] == '/')
					{
						if (childTag->name.substr(1) != newTag->name)
						{
							std::cerr << "Invalid closing tag: " << childTag->name;
							std::cerr << ". Expected: " << newTag->name << std::endl;
							throw std::exception();
						}
						else
						{
							isClosed = true;

						}
					}
					else
						newTag->children.push_back(childTag);
				}
			}
		}
		return newTag;
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
			std::cout << *child;
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