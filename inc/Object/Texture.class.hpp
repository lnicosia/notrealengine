#ifndef _TEXTURE_CLASS_H_
# define _TEXTURE_CLASS_H_

#include <set>
#include <string>

namespace notrealengine
{
	class Texture
	{
	public:
		Texture() = delete;
		Texture(Texture const& ref) = delete;
		Texture(Texture && ref) noexcept;
		Texture(std::string const& path, std::string  type);
		~Texture();
		Texture& operator=(Texture const& text) = delete;
		Texture& operator=(Texture && text) noexcept;

		//	Accessors

		[[nodiscard]] unsigned int const& getId() const;
		[[nodiscard]] std::string const& getType() const;

		//	Setters

		void	setType(std::string const& type);

	private:
		unsigned int	glId;
		std::string		type;

	};
} // namespace notrealengine

#endif