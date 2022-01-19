#ifndef _LIGHT_CLASS_H_
#define _LIGHT_CLASS_H_

#include "Object/Texture.class.hpp"
#include "Object/Transform.class.hpp"
#include "GLShaderProgram.class.hpp"

#include <memory>

enum LightType
{
	Directional,
	Sky,
	Spot,
	Point
};

namespace notrealengine
{
	class Light
	{
	public:
		Light(LightType type);
		~Light();

		void
			draw() const;

		void
			move(mft::vec3 translation);

		void
			setDirty(bool state);

		const Transform&
			getTransform() const;

		const bool
			isDirty() const;

	private:
		int	type;
		std::string	name;

		unsigned int	VBO, VAO, texture;

		unsigned int	shader;

		static unsigned int count;

		Transform transform;

		bool	dirty;

	};
}

#endif
