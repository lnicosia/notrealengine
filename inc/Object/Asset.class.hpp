#ifndef _ASSET_CLASS_H_
#define _ASSET_CLASS_H_

#include <string>
#include <filesystem>

namespace notrealengine
{
	class Asset
	{
	public:
		Asset(const std::filesystem::path& path);
		~Asset();

		//	Accessors

		virtual const std::string&
			getName() const;

		const uint32_t&
			getId() const;

		const std::filesystem::path&
			getPath() const;

		const bool
			isLoaded() const;

		virtual const std::string
			getAssetType() const = 0;

		//	Setters

		void
			setName(const std::string& name);

		//	No setter for ID

		void
			setPath(const std::filesystem::path& path);

		void
			setLoaded(bool state);

	protected:
		std::string				name;
		std::filesystem::path	path;
	private:
		
		uint32_t				id;
		bool					loaded;

		static uint32_t			count;
	};

	std::ostream& operator<<(std::ostream& o, Asset const& asset);
}
#endif // !_ASSET_CLASS_H_
