
#ifndef  __GLSHADERPROGRAM_CLASS_H__
# define __GLSHADERPROGRAM_CLASS_H__

# include "GL.hpp"

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
		~GLShaderProgram( void );

		const GLuint programID;
	};
}

#endif

