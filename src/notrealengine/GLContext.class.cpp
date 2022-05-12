
#include "GLContext.class.hpp"
#include "Object/AssetManager.class.hpp"
#include "Object/GLObject.class.hpp"
#include "CheckFileType.hpp"

#include <stdexcept>
#include <utility>
#include <fstream>
#include <sstream>
#include <iostream>
# define _USE_MATH_DEFINES_
#include <math.h>

namespace notrealengine
{
	std::shared_ptr<GLMesh> CreateSphere(float radius, unsigned int horizontal, unsigned int vertical)
	{
		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;
		float H = 1.0f / static_cast<float>(horizontal - 1.0f);
		float V = 1.0f / static_cast<float>(vertical - 1.0f);
		float lengthInv = 1.0f / radius;
		for (unsigned int h = 0; h < horizontal; h++)
		{
			for (unsigned int v = 0; v < vertical; v++)
			{
				//	+ radius to decenter the sphere
				mft::vec3 pos(cosf(2 * M_PI * v * V) * sinf(M_PI * h * H),
					sinf(-M_PI_2 + M_PI * h * H),
					sinf(2 * M_PI * v * V) * sinf(M_PI * h * H));
				mft::vec3 norm = pos * lengthInv;
				mft::vec2 tex(static_cast<float>(v) / static_cast<float>(vertical),
					static_cast<float>(h) / static_cast<float>(vertical));
				pos = pos * radius + radius;
				vertices.push_back(Vertex(pos, norm, tex));
				
				if (h < horizontal - 1)
				{
					unsigned int row = h * vertical;
					unsigned int nextRow = (h + 1) * vertical;
					unsigned int nextV = (v + 1) % vertical;

					indices.push_back(row + v);
					indices.push_back(nextRow + v);
					indices.push_back(nextRow + nextV);

					indices.push_back(row + v);
					indices.push_back(nextRow + nextV);
					indices.push_back(row + nextV);
				}
			}
		}
		MeshData	data(vertices, indices);
		std::vector<std::shared_ptr<Texture>> textures;
		return std::make_shared<GLMesh>(data, textures);
	}

	std::shared_ptr<GLMesh> CreateCenteredSphere(float radius, unsigned int horizontal, unsigned int vertical)
	{
		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;
		float H = 1.0f / static_cast<float>(horizontal - 1.0f);
		float V = 1.0f / static_cast<float>(vertical - 1.0f);
		float lengthInv = 1.0f / radius;
		for (unsigned int h = 0; h < horizontal; h++)
		{
			for (unsigned int v = 0; v < vertical; v++)
			{
				//	+ radius to decenter the sphere
				mft::vec3 pos(cosf(2 * M_PI * v * V) * sinf(M_PI * h * H),
					sinf(-M_PI_2 + M_PI * h * H),
					sinf(2 * M_PI * v * V) * sinf(M_PI * h * H));
				mft::vec3 norm = pos * lengthInv;
				mft::vec2 tex(static_cast<float>(v) / static_cast<float>(vertical),
					static_cast<float>(h) / static_cast<float>(vertical));
				pos = pos * radius;
				vertices.push_back(Vertex(pos, norm, tex));

				if (h < horizontal - 1)
				{
					unsigned int row = h * vertical;
					unsigned int nextRow = (h + 1) * vertical;
					unsigned int nextV = (v + 1) % vertical;

					indices.push_back(row + v);
					indices.push_back(nextRow + v);
					indices.push_back(nextRow + nextV);

					indices.push_back(row + v);
					indices.push_back(nextRow + nextV);
					indices.push_back(row + nextV);
				}
			}
		}
		MeshData	data(vertices, indices);
		std::vector<std::shared_ptr<Texture>> textures;
		return std::make_shared<GLMesh>(data, textures);
	}

	GLContext::GLContext()
	{
		registerShader("color", "shaders/solid.vs", "shaders/color.fs");
		registerShader("colorUnlit", "shaders/solid.vs", "shaders/colorUnlit.fs");
		registerShader("bonesInfluence", "shaders/skeletal.vs", "shaders/bonesInfluence.fs");
		registerShader("default", "shaders/skeletal.vs", "shaders/default.fs");
		registerShader("unlit", "shaders/skeletal.vs", "shaders/unlit.fs");
		registerShader("2dProjected", "shaders/2dProjected.vs", "shaders/2dProjected.fs");
		registerShader("2d", "shaders/2d.vs", "shaders/2d.fs");
		registerShader("skybox", "shaders/skybox.vs", "shaders/skybox.fs");
		registerShader("text", "shaders/2d.vs", "shaders/text.fs");

		std::vector<Vertex>	vertices;

		vertices.push_back(Vertex(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f));
		vertices.push_back(Vertex(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f));
		vertices.push_back(Vertex(1.0f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f));
		vertices.push_back(Vertex(1.0f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f));
		vertices.push_back(Vertex(0.0f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f));
		vertices.push_back(Vertex(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f));

		vertices.push_back(Vertex(0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f));
		vertices.push_back(Vertex(1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f));
		vertices.push_back(Vertex(1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f));
		vertices.push_back(Vertex(1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f));
		vertices.push_back(Vertex(0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f));
		vertices.push_back(Vertex(0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f));

		vertices.push_back(Vertex(0.0f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f));
		vertices.push_back(Vertex(0.0f, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f));
		vertices.push_back(Vertex(0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f));
		vertices.push_back(Vertex(0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f));
		vertices.push_back(Vertex(0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f));
		vertices.push_back(Vertex(0.0f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f));

		vertices.push_back(Vertex(1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f));
		vertices.push_back(Vertex(1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f));
		vertices.push_back(Vertex(1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f));
		vertices.push_back(Vertex(1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f));
		vertices.push_back(Vertex(1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f));
		vertices.push_back(Vertex(1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f));

		vertices.push_back(Vertex(0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f));
		vertices.push_back(Vertex(1.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f));
		vertices.push_back(Vertex(1.0f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f));
		vertices.push_back(Vertex(1.0f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f));
		vertices.push_back(Vertex(0.0f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f));
		vertices.push_back(Vertex(0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f));

		vertices.push_back(Vertex(0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f));
		vertices.push_back(Vertex(1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f));
		vertices.push_back(Vertex(1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f));
		vertices.push_back(Vertex(1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f));
		vertices.push_back(Vertex(0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f));
		vertices.push_back(Vertex(0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f));
		std::vector<unsigned int> indices;
		for (unsigned int i = 0; i < vertices.size(); i++)
		{
			indices.push_back(i);
		}

		MeshData	data(vertices, indices);
		std::vector<std::shared_ptr<Texture>> textures;
		cube = std::make_shared<GLMesh>(data, textures);

		vertices.clear();
		vertices.push_back(Vertex(-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f));
		vertices.push_back(Vertex(0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f));
		vertices.push_back(Vertex(0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f));
		vertices.push_back(Vertex(0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f));
		vertices.push_back(Vertex(-0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f));
		vertices.push_back(Vertex(-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f));

		vertices.push_back(Vertex(-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f));
		vertices.push_back(Vertex(0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f));
		vertices.push_back(Vertex(0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f));
		vertices.push_back(Vertex(0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f));
		vertices.push_back(Vertex(-0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f));
		vertices.push_back(Vertex(-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f));

		vertices.push_back(Vertex(-0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f));
		vertices.push_back(Vertex(-0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f));
		vertices.push_back(Vertex(-0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f));
		vertices.push_back(Vertex(-0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f));
		vertices.push_back(Vertex(-0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f));
		vertices.push_back(Vertex(-0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f));

		vertices.push_back(Vertex(0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f));
		vertices.push_back(Vertex(0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f));
		vertices.push_back(Vertex(0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f));
		vertices.push_back(Vertex(0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f));
		vertices.push_back(Vertex(0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f));
		vertices.push_back(Vertex(0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f));

		vertices.push_back(Vertex(-0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f));
		vertices.push_back(Vertex(0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f));
		vertices.push_back(Vertex(0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f));
		vertices.push_back(Vertex(0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f));
		vertices.push_back(Vertex(-0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f));
		vertices.push_back(Vertex(-0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f));

		vertices.push_back(Vertex(-0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f));
		vertices.push_back(Vertex(0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f));
		vertices.push_back(Vertex(0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f));
		vertices.push_back(Vertex(0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f));
		vertices.push_back(Vertex(-0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f));
		vertices.push_back(Vertex(-0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f));
		indices.clear();
		for (unsigned int i = 0; i < vertices.size(); i++)
		{
			indices.push_back(i);
		}

		MeshData	data1(vertices, indices);
		textures.clear();
		centeredCube = std::make_shared<GLMesh>(data1, textures);

		vertices.clear();
		vertices.push_back(Vertex(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f));
		vertices.push_back(Vertex(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f));
		vertices.push_back(Vertex(1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f));
		vertices.push_back(Vertex(1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f));
		vertices.push_back(Vertex(0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f));
		vertices.push_back(Vertex(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f));

		MeshData	data2(vertices, indices);
		std::vector<std::shared_ptr<Texture>> textures2;
		std::shared_ptr<GLMesh> square = std::make_shared<GLMesh>(data2, textures2);
		std::vector<std::shared_ptr<Mesh>>	meshes;
		for (int i = 0; i < 10; i++)
		{
			for (int j = 0; j < 10; j++)
			{
				std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>(square);
				mesh->setName("Grid component ["
				+ std::to_string(i) + "][" + std::to_string(j) + "]");
				mesh->localTransform.move(mft::vec3(i, 0.0f, j));
				mesh->setColor(mft::vec4(1.0f, 1.0f, 1.0f, 1.0f));
				meshes.push_back(mesh);
				std::shared_ptr<Mesh> mesh2 = std::make_shared<Mesh>(square);
				mesh2->setName("Grid component ["
				+ std::to_string(-i) + "][" + std::to_string(-j) + "]");
				mesh2->localTransform.move(mft::vec3(-i, 0.0f, -j));
				mesh2->setColor(mft::vec4(1.0f, 1.0f, 1.0f, 1.0f));
				meshes.push_back(mesh2);
				std::shared_ptr<Mesh> mesh3 = std::make_shared<Mesh>(square);
				mesh3->setName("Grid component ["
				+ std::to_string(i) + "][" + std::to_string(-j) + "]");
				mesh3->localTransform.move(mft::vec3(i, 0.0f, -j));
				mesh3->setColor(mft::vec4(1.0f, 1.0f, 1.0f, 1.0f));
				meshes.push_back(mesh3);
				std::shared_ptr<Mesh> mesh4 = std::make_shared<Mesh>(square);
				mesh4->setName("Grid component ["
				+ std::to_string(-i) + "][" + std::to_string(j) + "]");
				mesh4->localTransform.move(mft::vec3(-i, 0.0f, j));
				mesh4->setColor(mft::vec4(1.0f, 1.0f, 1.0f, 1.0f));
				meshes.push_back(mesh4);
			}
		}

		grid = std::make_shared<GLObject>(meshes);
		grid->setName("Grid");
		grid->setShader(this->getShader("colorUnlit")->programID);

		sphere = CreateSphere(0.5f, 15, 15);
	}

	GLContext::~GLContext()
	{
		cube.reset();
		centeredCube.reset();
		sphere.reset();
		grid.reset();
		shaders.clear();
	}

	void GLContext::registerShader( std::string name, std::filesystem::path vertex, std::filesystem::path fragment )
	{
		std::cout << "Loading shader " << vertex << ", " << fragment << " as \"";
 		std::cout << name << "\"";

		if (!IsReg(vertex))
			throw std::invalid_argument("\nnre: Invalid vertex shader file type");

		if (!IsReg(fragment))
			throw std::invalid_argument("\nnre: Invalid fragment shader file type");

		if (shaders.contains(name))
			throw std::invalid_argument( "Shader '" + name + "' has already been registered!" );
		std::string
			vert_code, frag_code;
		std::ifstream
			vert_file, frag_file;

		vertex.make_preferred();
		fragment.make_preferred();

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
		std::cout << " (id = " << getShader(name)->programID << ")" << std::endl;
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
	std::shared_ptr<GLMesh>	GLContext::sphere = std::shared_ptr<GLMesh>();
	std::shared_ptr<GLMesh>	GLContext::centeredCube = std::shared_ptr<GLMesh>();
	std::shared_ptr<GLObject>	GLContext::grid = std::shared_ptr<GLObject>();
	long GLContext::CurrentContext = 0;

}
