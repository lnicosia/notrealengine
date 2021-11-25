
#include "GLContext.class.hpp"

#include <stdexcept>
#include <utility>
#include <fstream>
#include <sstream>

namespace notrealengine
{
	GLContext::GLContext()
	{

	}

	GLContext::~GLContext()
	{

	}

	void GLContext::registerShader( std::string name, std::filesystem::path vertex, std::filesystem::path fragment )
	{
		if (shaders.contains(name))
			throw std::invalid_argument( "Shader '" + name + "' has already been registered!" );
		std::string
			vert_code, frag_code;
		std::ifstream
			vert_file, frag_file;

		vert_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		try {
			vert_file.open(vertex);
			std::stringstream vert_in_stream;
			vert_in_stream << vert_file.rdbuf();
			vert_file.close();

			vert_code = vert_in_stream.str();
		} catch (std::ifstream::failure & e) {
			throw std::runtime_error( "Could not read shader files '" + vertex.string() + "'" );
		}

		frag_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		try {
			frag_file.open(vertex);
			std::stringstream frag_in_stream;
			frag_in_stream << frag_file.rdbuf();
			frag_file.close();

			frag_code = frag_in_stream.str();
		} catch (std::ifstream::failure & e) {
			throw std::runtime_error( "Could not read shader files '" + fragment.string() + "'" );
		}

		if (!isCurrent())
			makeCurrent();
		shaders.emplace(std::piecewise_construct,
				std::forward_as_tuple(name),
				std::forward_as_tuple(
					GLShaderProgram::ShaderPart<GL_VERTEX_SHADER>(vert_code),
					GLShaderProgram::ShaderPart<GL_FRAGMENT_SHADER>(frag_code)
					)
				);
	}

	GLShaderProgram * GLContext::getShader( std::string name )
	{
		if (!shaders.contains(name)) {
			try {
				registerShader(name, DefaultShaderPath / (name + ".vs"), DefaultShaderPath / (name + ".fs"));
			} catch (std::exception & e) {
				throw std::invalid_argument( "Shader '" + name + "' has not been registered yet, and failed to construct it!" );
			}
		}
		return &shaders.at(name);
	}

	std::filesystem::path GLContext::DefaultShaderPath = "shaders/";
	long GLContext::CurrentContext = 0;
}
