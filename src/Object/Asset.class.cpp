#include "Object/Asset.class.hpp"

#include <iostream>

namespace notrealengine
{
	Asset::Asset(const std::filesystem::path& path): path(path), loaded(false)
	{
		id = count;
		count++;
		this->path.make_preferred();
		name = this->path.filename().string();
	}

	Asset::~Asset()
	{

	}

	//	Accessors

	const std::string& Asset::getName() const
	{
		return name;
	}

	const uint32_t& Asset::getId() const
	{
		return id;
	}

	const std::filesystem::path& Asset::getPath() const
	{
		return path;
	}

	const bool	Asset::isLoaded() const
	{
		return loaded;
	}

	//	Setters

	void	Asset::setName(const std::string& name)
	{
		this->name = name;
	}

	void	Asset::setPath(const std::filesystem::path& path)
	{
		this->path = path;
	}

	void	Asset::setLoaded(bool state)
	{
		this->loaded = state;
	}

	std::ostream& operator<<(std::ostream& o, Asset const& asset)
	{
		std::cout << "- " << asset.getAssetType() << " [" << asset.getName();
		std::cout << "]: ID = " << asset.getId() << ", path:";
		std::cout << asset.getPath();
		if (asset.isLoaded())
			std::cout << ",is loaded";
		else
			std::cout << ",is not loaded";
		std::cout << std::endl;
		return o;
	}

	uint32_t	Asset::count = 0;
}