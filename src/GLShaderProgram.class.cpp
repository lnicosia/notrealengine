
#include "GLShaderProgram.class.hpp"

#include <iostream>

namespace nre
{
	GLShaderProgram::GLShaderProgram( ShaderPart<GL_VERTEX_SHADER> vertex,
									  ShaderPart<GL_FRAGMENT_SHADER> fragment ):
		programID(GlCallThrow(glCreateProgram, (Type)))
	{
		try {
			GL_CALL_THROW(glAttachShader, (programID, vert_shader));
			GL_CALL_THROW(glAttachShader, (programID, frag_shader));
			GL_CALL_THROW(glLinkProgram, (programID));
		}
	}

	GLShaderProgram::~GLShaderProgram( void )
	{
		try {
			GL_CALL_THROW(glDeleteProgram, (programID));
		} catch (std::exception e) {
			std::cerr << "Exception caught in destructor (!) :" << std::endl << e.what();
		}
	}

	template<GLenum Type>
	GLShaderProgram::ShaderPart::ShaderPart( std::string code ):
		shaderID(GlCallThrow(glCreateShader, (Type)))
	{
		try {
			GL_CALL_THROW(glShaderSource, (shaderID, 1, &code, (int *){std::size(code)}));
			GL_CALL_THROW(glCompileShader, (shaderID));
		} catch (std::exception e) {
			GL_CALL_THROW(glDeleteShader, (shaderID));
		}
	}

	template<GLenum Type>
	GLShaderProgram::ShaderPart::~ShaderPart( void )
	{
		try {
			GL_CALL_THROW(glDeleteShader, (shaderID));
		} catch (std::exception e) {
			std::cerr << "Exception caught in destructor (!) :" << std::endl << e.what();
		}
	}

	template<GL_VERTEX_SHADER> class ShaderPart;
	template<GL_FRAGMENT_SHADER> class ShaderPart;
}
