
#ifndef  __GLSHADERPROGRAM_CLASS_H__
# define __GLSHADERPROGRAM_CLASS_H__

# include "GL.hpp"
# include "mft/mft.hpp"
# include "Object/Asset.class.hpp"

namespace notrealengine
{
	class GLShaderProgram
	{
	public:
		template <GLenum Type>
		class ShaderPart {
		public:
			ShaderPart( std::string code );
			~ShaderPart( void );

			const GLuint shaderID;
		};


		GLShaderProgram( ShaderPart<GL_VERTEX_SHADER> vertex,
						 ShaderPart<GL_FRAGMENT_SHADER> fragment );
		//GLShaderProgram(std::string vertexPath,
		//				 std::string fragmentPath, std::string name);
		~GLShaderProgram( void );

		void
			bindMatrix(std::string name, const mft::mat4& mat) const;
		void
			bindFloat(std::string name, const float nb) const;
		void
			bindInt(std::string name, const int nb) const;

		const GLuint programID;

	};

	void
		bindMatrix(unsigned int shader, std::string name, const mft::mat4& mat);
	void
		bindFloat(unsigned int shader, std::string name, const float nb);
	void
		bindInt(unsigned int shader, std::string name, const int nb);
}

#endif
