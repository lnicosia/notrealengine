#ifndef _ASSET_CLASS_H_
#define _ASSET_CLASS_H_

#include <string>
#include <filesystem>
#include <vector>

namespace notrealengine
{
	class Asset
	{
	public:
		Asset() = delete;
		Asset(const std::vector<std::filesystem::path>& paths);
		Asset(const Asset& ref) = delete;
		Asset(Asset&& ref);
		Asset& operator=(const Asset& ref) = delete;
		Asset& operator=(Asset&& ref);
		~Asset();

		//	Accessors

		virtual const std::string&
			getName() const;

		const uint32_t&
			getId() const;

		const std::filesystem::path&
			getPath(int index = 0) const;

		const std::vector<std::filesystem::path>&
			getPaths() const;

		const bool
			isLoaded() const;

		virtual const std::string
			getAssetType() const = 0;

		//	Setters

		void
			setName(const std::string& name);

		//	No setter for ID

		void
			setPath(const std::filesystem::path& path, int index = 0);

		void
			setPaths(const std::vector<std::filesystem::path>& paths);

	protected:
		std::string				name;
		/**	Some assets (including Shaders) may have 2 or even more paths defining them
		**	In this case all of their paths will be checked when requesting them
		*/
		std::vector<std::filesystem::path> paths;
	private:

		uint32_t				id;
		bool					loaded;

		static uint32_t			count;
	};

	std::ostream& operator<<(std::ostream& o, Asset const& asset);
}
#endif // !_ASSET_CLASS_H_
