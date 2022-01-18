
#include "GLContext.class.hpp"

#include <stdexcept>
#include <utility>
#include <fstream>
#include <sstream>

namespace notrealengine
{
	GLContext::GLContext()
	{
		std::vector<Vertex>	vertices;

		vertices.push_back(Vertex(-1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f));
		vertices.push_back(Vertex(1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f));
		vertices.push_back(Vertex(1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f));
		vertices.push_back(Vertex(1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f));
		vertices.push_back(Vertex(-1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f));
		vertices.push_back(Vertex(-1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f));

		vertices.push_back(Vertex(-1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f));
		vertices.push_back(Vertex(1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f));
		vertices.push_back(Vertex(1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f));
		vertices.push_back(Vertex(1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f));
		vertices.push_back(Vertex(-1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f));
		vertices.push_back(Vertex(-1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f));

		vertices.push_back(Vertex(-1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f));
		vertices.push_back(Vertex(-1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f));
		vertices.push_back(Vertex(-1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f));
		vertices.push_back(Vertex(-1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f));
		vertices.push_back(Vertex(-1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f));
		vertices.push_back(Vertex(-1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f));

		vertices.push_back(Vertex(1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f));
		vertices.push_back(Vertex(1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f));
		vertices.push_back(Vertex(1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f));
		vertices.push_back(Vertex(1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f));
		vertices.push_back(Vertex(1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f));
		vertices.push_back(Vertex(1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f));

		vertices.push_back(Vertex(-1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f));
		vertices.push_back(Vertex(1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f));
		vertices.push_back(Vertex(1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f));
		vertices.push_back(Vertex(1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f));
		vertices.push_back(Vertex(-1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f));
		vertices.push_back(Vertex(-1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f));

		vertices.push_back(Vertex(-1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f));
		vertices.push_back(Vertex(1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f));
		vertices.push_back(Vertex(1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f));
		vertices.push_back(Vertex(1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f));
		vertices.push_back(Vertex(-1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f));
		vertices.push_back(Vertex(-1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f));
		std::vector<unsigned int> indices;
		for (unsigned int i = 0; i < vertices.size(); i++)
		{
			indices.push_back(i);
		}

		MeshData	data(vertices, indices);
		std::vector<std::shared_ptr<Texture>> textures;
		cube = std::shared_ptr<GLMesh>(new GLMesh(data, textures));

		registerShader("color", "shaders/test.vs", "shaders/colorShader.fs");
		registerShader("bones", "shaders/anim.vs", "shaders/bones.fs");
		registerShader("anim", "shaders/anim.vs", "shaders/test.fs");
		//context.registerShader("default", "shaders/anim.vs", "shaders/test.fs");
		registerShader("2dProjected", "shaders/2dProjected.vs", "shaders/2dProjected.fs");

		registerShader("text", "shaders/text.vs", "shaders/text.fs");
	}

	GLContext::~GLContext()
	{

	}

	void GLContext::registerShader( std::string name, std::filesystem::path vertex, std::filesystem::path fragment )
	{
		std::cout << "Loading shader " << vertex << ", " << fragment << " as \"";
 		std::cout << name << "\"" << std::endl;
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
			frag_file.open(fragment);
			std::stringstream frag_in_stream;
			frag_in_stream << frag_file.rdbuf();
			frag_file.close();

			frag_code = frag_in_stream.str();
		} catch (std::ifstream::failure & e) {
			throw std::runtime_error( "Could not read shader files '" + fragment.string() + "'" );
		}

		//if (!isCurrent())
		//	makeCurrent();
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
	std::map<std::string, GLShaderProgram> GLContext::shaders = std::map<std::string, GLShaderProgram>();
	std::shared_ptr<GLMesh>	GLContext::cube = std::shared_ptr<GLMesh>();
	long GLContext::CurrentContext = 0;

}
