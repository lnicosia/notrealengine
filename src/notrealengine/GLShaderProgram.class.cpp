
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
			int	success;
			char	infoLog[512];
			GLCallThrow(glGetProgramiv, programID, GL_LINK_STATUS, &success);
			if (!success)
			{
				GLCallThrow(glGetProgramInfoLog, programID, 512, NULL, infoLog);
				std::cerr << "Failed to link shader:" << infoLog << std::endl;
			}
		} catch (std::exception e) {
			GLCallThrow(glDeleteProgram, programID);
		}
	}

	GLShaderProgram::~GLShaderProgram( void )
	{
		GLCallThrow(glDeleteProgram, (programID));
	}

	template<GLenum Type>
	GLShaderProgram::ShaderPart<Type>::ShaderPart( std::string code ):
		shaderID(GLCallThrow(glCreateShader, Type))
	{
		try {
			const char* str = code.c_str();
			GLCallThrow(glShaderSource, shaderID, 1, &str, NULL);
			GLCallThrow(glCompileShader, shaderID);
			int	success;
			char	infoLog[512];
			GLCallThrow(glGetShaderiv, shaderID, GL_COMPILE_STATUS, &success);
			if (!success)
			{
				GLCallThrow(glGetShaderInfoLog, shaderID, 512, NULL, infoLog);
				std::cerr << "Failed to compile shader:" << infoLog << std::endl;
			}
		} catch (std::exception e) {
			GLCallThrow(glDeleteShader, shaderID);
		}
	}

	template<GLenum Type>
	GLShaderProgram::ShaderPart<Type>::~ShaderPart( void )
	{
		try {
			GLCallThrow(glDeleteShader, shaderID);
			int	success;
			char	infoLog[512];
			GLCallThrow(glGetShaderiv, shaderID, GL_DELETE_STATUS, &success);
			if (!success)
			{
				GLCallThrow(glGetShaderInfoLog, shaderID, 512, NULL, infoLog);
				std::cerr << "Failed to delete shader:" << infoLog << std::endl;
			}
		} catch (std::exception e) {
			std::cerr << "Exception caught in destructor (!) :" << std::endl << e.what();
		}
	}

	void	GLShaderProgram::bindMatrix(std::string name, const mft::mat4& mat) const
	{
		GLCallThrow(glUseProgram, this->programID);
		GLint location = GLCallThrow(glGetUniformLocation, this->programID, name.c_str());
		GLCallThrow(glUniformMatrix4fv, location, 1, GL_TRUE, static_cast<const float*>(mat));
	}

	void	GLShaderProgram::bindFloat(std::string name, const float nb) const
	{
		GLCallThrow(glUseProgram, this->programID);
		GLint location = GLCallThrow(glGetUniformLocation, this->programID, name.c_str());
		GLCallThrow(glUniform1f, location, nb);
	}

	void	GLShaderProgram::bindInt(std::string name, const int nb) const
	{
		GLCallThrow(glUseProgram, this->programID);
		GLint location = GLCallThrow(glGetUniformLocation, this->programID, name.c_str());
		GLCallThrow(glUniform1i, location, nb);
	}

	void	GLShaderProgram::bindVector(std::string name, const mft::vec3& vec) const
	{
		GLCallThrow(glUseProgram, this->programID);
		GLint location = GLCallThrow(glGetUniformLocation, this->programID, name.c_str());
		GLCallThrow(glUniform3f, location, vec.x, vec.y, vec.z);
	}

	void	GLShaderProgram::bindVector(std::string name, const mft::vec4& vec) const
	{
		GLCallThrow(glUseProgram, this->programID);
		GLint location = GLCallThrow(glGetUniformLocation, this->programID, name.c_str());
		GLCallThrow(glUniform4f, location, vec.x, vec.y, vec.z, vec.w);
	}

	template class GLShaderProgram::ShaderPart<GL_VERTEX_SHADER>;
	template class GLShaderProgram::ShaderPart<GL_FRAGMENT_SHADER>;

	void	bindMatrix(unsigned int shader, std::string name, const mft::mat4& mat)
	{
		GLCallThrow(glUseProgram, shader);
		GLint location = GLCallThrow(glGetUniformLocation, shader, name.c_str());
		GLCallThrow(glUniformMatrix4fv, location, 1, GL_TRUE, static_cast<const float*>(mat));
	}

	void	bindFloat(unsigned int shader, std::string name, const float nb)
	{
		GLCallThrow(glUseProgram, shader);
		GLint location = GLCallThrow(glGetUniformLocation, shader, name.c_str());
		GLCallThrow(glUniform1f, location, nb);
	}

	void	bindInt(unsigned int shader, std::string name, const int nb)
	{
		GLCallThrow(glUseProgram, shader);
		GLint location = GLCallThrow(glGetUniformLocation, shader, name.c_str());
		GLCallThrow(glUniform1i, location, nb);
	}

	void	bindVector(unsigned int shader, std::string name, const mft::vec3& vec)
	{
		GLCallThrow(glUseProgram, shader);
		GLint location = GLCallThrow(glGetUniformLocation, shader, name.c_str());
		GLCallThrow(glUniform3f, location, vec.x, vec.y, vec.z);
	}

	void	bindVector(unsigned int shader, std::string name, const mft::vec4& vec)
	{
		GLCallThrow(glUseProgram, shader);
		GLint location = GLCallThrow(glGetUniformLocation, shader, name.c_str());
		GLCallThrow(glUniform4f, location, vec.x, vec.y, vec.z, vec.w);
	}
}
