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
	class Light: public Asset
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

		//	Asset

		const std::string
			getAssetType() const override;

	private:
		int	type;

		unsigned int	VBO, VAO, texture;

		unsigned int	shader;

		static unsigned int count;

		Transform transform;

		bool	dirty;

	};
}

#endif
