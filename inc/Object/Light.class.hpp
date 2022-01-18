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

		Transform transform;

		void
			draw() const;

	private:
		int	type;
		std::string	name;

		unsigned int	VBO, VAO, texture;

		static unsigned int count;

	};
}

#endif
