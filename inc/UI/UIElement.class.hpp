#ifndef _UIELEMENT_CLASS_H_
#define _UIELEMENT_CLASS_H_

#include "mft/mft.hpp"
#include "Object/TextureLoader.class.hpp"
#include "GLContext.class.hpp"
#include "Inputs/Binding.class.hpp"

#include <vector>

namespace notrealengine
{
	class UIElement
	{
	public:
		UIElement(GLShaderProgram* shader = nullptr,
			std::shared_ptr<Texture> image = TextureLoader::loadTexture("resources/UI/defaultUI-hovered.png", "UI"),
			mft::vec2i pos = mft::vec2i(0, 0));
		~UIElement();

		bool	active;
		bool	visible;

		constexpr void
			enable();
		constexpr void
			disable();
		constexpr void
			show();
		constexpr void
			hide();

		virtual void
			draw() const;

		/** Update element's and children's state if their logic needs to
		*/
		virtual void
			update(const mft::vec2i& mousePos, const InputState mouseState);

		//	Getters

		const mft::vec2i&
			getPos() const;

		//	Setters

		virtual void
			setPos(const mft::vec2i&& newPos);

	protected:
		mft::vec2i	pos;
		mft::vec2i	size;

		GLShaderProgram*			shader;
		std::shared_ptr<Texture>	image;

		std::vector<std::shared_ptr<UIElement>>	children;
	};
}
#endif // !_UIELEMENT_CLASS_H_
