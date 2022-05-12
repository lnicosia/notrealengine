#include "Object/Asset.class.hpp"

#include <iostream>

namespace notrealengine
{
	Asset::Asset(const std::vector<std::filesystem::path>& paths)
	: paths(paths), loaded(true)
	{
		id = count;
		count++;
		for (int i = 0; i < paths.size(); i++)
			this->paths[i].make_preferred();
		name = this->paths[0].filename().string();
	}

	Asset::Asset(const std::string& name): paths({""}), name(name), loaded(true)
	{
		id = count;
		count++;
	}

	Asset::Asset(): paths({""}), name(), loaded(true)
	{
		id = count;
		count++;
	}

	Asset::Asset(Asset&& ref):
		name(std::move(ref.name)),
		paths(std::move(ref.paths)),
		id(std::exchange(ref.id, 0)),
		loaded(std::exchange(ref.loaded, false))
	{

	}

	Asset&	Asset::operator=(Asset&& ref)
	{
		this->name = std::move(ref.name);
		this->paths = std::move(ref.paths);
		this->id = std::exchange(ref.id, 0);
		this->loaded = std::exchange(ref.loaded, false);
		return *this;
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

	const std::filesystem::path& Asset::getPath(int index) const
	{
		if (index >= paths.size())
		{
			std::cerr << "Error: requesting path #" << index << " of array of size ";
			std::cerr << paths.size() << std::endl;
		}
		return paths[index];
	}

	const std::vector<std::filesystem::path>& Asset::getPaths() const
	{
		return paths;
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

	void	Asset::setPath(const std::filesystem::path& path, int index)
	{
		if (index >= paths.size())
		{
			std::cerr << "Error: setting path #" << index << " of array of size ";
			std::cerr << paths.size() << std::endl;
		}
		this->paths[index] = path;
	}

	void	Asset::setPaths(const std::vector<std::filesystem::path>& paths)
	{
		this->paths = paths;
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
