
#ifndef  __GLSHADERPROGRAM_CLASS_H__
# define __GLSHADERPROGRAM_CLASS_H__

# include "GL.hpp"

namespace notrealengine
{
	class GLShaderProgram {
	public:
		template <GLenum Type>
		class ShaderPart {
		public:
			explicit ShaderPart( const std::string& code );
			~ShaderPart( );

			const GLuint shaderID;
		};


		GLShaderProgram( const ShaderPart<GL_VERTEX_SHADER>& vertex,
						 const ShaderPart<GL_FRAGMENT_SHADER>& fragment );
		~GLShaderProgram( );

		const GLuint programID;
	};
} // namespace notrealengine

#endif

