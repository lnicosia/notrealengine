#ifndef _UIELEMENT_CLASS_H_
#define _UIELEMENT_CLASS_H_

#include "mft/mft.hpp"
#include "Object/AssetManager.class.hpp"
#include "GLContext.class.hpp"
#include "Inputs/Binding.class.hpp"
#include "TextRendering/GLFont.class.hpp"

#include <vector>

namespace notrealengine
{

	struct UIText
	{
		UIText(std::string text, mft::vec2i pos = mft::vec2i(0, 0), float scale = 1.0f,
			const mft::vec4& color = mft::vec4(1.0f),
			std::shared_ptr<GLFont> font =
#ifdef USING_EXTERNAL_LIBS
				AssetManager::getInstance().loadAsset<GLFont>("resources/fonts/arial.ttf")):
#else
				AssetManager::getInstance().loadAsset<GLFont>("resources/fonts/pt-sans-48.bff")):
#endif
			text(text), pos(pos), scale(scale), color(color), font(font)
		{
#ifdef USING_EXTERNAL_LIBS
			this->scale *= 0.55f;
#endif
		}

		UIText() = delete;
		~UIText()
		{
		}
		std::string				text;
		mft::vec2i				pos;
		float					scale;
		mft::vec4				color;
		std::shared_ptr<GLFont>	font;
	};

	class UIElement
	{
	public:
		UIElement(mft::vec2i pos = mft::vec2i(0, 0),
		std::shared_ptr<Texture> image =
		AssetManager::getInstance().loadAsset<Texture>("resources/UI/defaultUI.png", "UI"),
		GLShaderProgram* shader = GLContext::getShader("2d"));
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

		const mft::vec2i&
			getSize() const;

		std::shared_ptr<UIElement>
			getChild(size_t index = 0);

		const std::vector<std::shared_ptr<UIElement>>&
			getChildren();

		//	Setters

		virtual void
			setPos(const mft::vec2i&& newPos);

		virtual void
			addPos(const mft::vec2i& pos);

		virtual void
			setSize(const mft::vec2i&& newSize);

		void
			setImage(std::shared_ptr<Texture> image);

		void
			addChild(std::shared_ptr<UIElement> child);

		/**	Release the shared ptrs
		*/
		void
			clearChildren();

		std::vector<UIText>	texts;

	protected:
		mft::vec2i	pos;
		mft::vec2i	size;

		GLShaderProgram*			shader;
		std::shared_ptr<Texture>	image;

		std::vector<std::shared_ptr<UIElement>>	children;
	};
}
#endif // !_UIELEMENT_CLASS_H_
