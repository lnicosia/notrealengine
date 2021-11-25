
#include "GLShaderProgram.class.hpp"

#include <iostream>

namespace notrealengine
{
	GLShaderProgram::GLShaderProgram( ShaderPart<GL_VERTEX_SHADER> vertex,
									  ShaderPart<GL_FRAGMENT_SHADER> fragment ):
		programID(GLCallThrow(glCreateProgram))
	{
		try {
			GLCallThrow(glAttachShader, programID, vertex.shaderID);
			GLCallThrow(glAttachShader, programID, fragment.shaderID);
			GLCallThrow(glLinkProgram, programID);
		} catch (std::exception e) {
			GLCallThrow(glDeleteProgram, programID);
		}
	}

	GLShaderProgram::~GLShaderProgram( void )
	{
		try {
			GLCallThrow(glDeleteProgram, (programID));
		} catch (std::exception e) {
			std::cerr << "Exception caught in destructor (!) :" << std::endl << e.what();
		}
	}

	template<GLenum Type>
	GLShaderProgram::ShaderPart<Type>::ShaderPart( std::string code ):
		shaderID(GLCallThrow(glCreateShader, Type))
	{
		try {
			char* string[1] = { code.data() };
			GLint length[1] = { (GLint)std::size(code) };
			GLCallThrow(glShaderSource, shaderID, 1, string, length);
			GLCallThrow(glCompileShader, shaderID);
		} catch (std::exception e) {
			GLCallThrow(glDeleteShader, shaderID);
		}
	}

	template<GLenum Type>
	GLShaderProgram::ShaderPart<Type>::~ShaderPart( void )
	{
		try {
			GLCallThrow(glDeleteShader, shaderID);
		} catch (std::exception e) {
			std::cerr << "Exception caught in destructor (!) :" << std::endl << e.what();
		}
	}

	template class GLShaderProgram::ShaderPart<GL_VERTEX_SHADER>;
	template class GLShaderProgram::ShaderPart<GL_FRAGMENT_SHADER>;
}
