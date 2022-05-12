#ifndef _SKYBOX_CLASS_H_
#define _SKYBOX_CLASS_H_

#include "Object/Asset.class.hpp"
#include "GLShaderProgram.class.hpp"

namespace notrealengine
{
	class Skybox: public Asset
	{
	public:
		Skybox() = delete;
		Skybox(const Skybox& ref) = delete;
		Skybox& operator=(const Skybox& ref) = delete;
		Skybox(Skybox&& ref) noexcept;
		Skybox& operator=(Skybox&& ref);

		/**	Construct a skybox from 6 paths
		**
		*/
		Skybox(const std::string& path, const std::string paths[6]);

		~Skybox();

		//	Accessors

		const unsigned int&
			getGLId() const;
		const GLShaderProgram*
			getShader() const;
		const unsigned int
			getShaderID() const;

		//	Setters

		void
			setShader(GLShaderProgram* shader);

		//	Asset class

		const std::string
			getAssetType() const override;

		//	Draw

		/**	Draw the skybox
		*/
		void
			draw( void ) const;

	private:
		unsigned int VAO, VBO, glId;
		GLShaderProgram* shader;
	};
}

#endif // !_SKYBOX_CLASS_H_