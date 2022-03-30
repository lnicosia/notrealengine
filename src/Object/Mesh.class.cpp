#include "Object/Mesh.class.hpp"
#include "GLContext.class.hpp"

namespace notrealengine
{
	Mesh::Mesh(std::shared_ptr<GLMesh> const& glMesh)
		: name(glMesh->getName()), transform(), globalTransform(),
		glMesh(glMesh), animDirty(false),
		parentMatrix(), globalMatrix(), animMatrix(), normalMatrix(),
		shader(GLContext::getShader("default")->programID),
		color(mft::vec3(0.239f, 0.282f, 0.286f))
	{

	}

	Mesh::~Mesh()
	{

	}

	//	Accessors

	std::shared_ptr<GLMesh> const& Mesh::getGLMesh() const
	{
		return glMesh;
	}

	std::vector<std::shared_ptr<Mesh>> const& Mesh::getChildren() const
	{
		return children;
	}

	std::string const& Mesh::getName() const
	{
		return name;
	}

	mft::vec3 const& Mesh::getColor() const
	{
		return color;
	}

	unsigned int const& Mesh::getShader() const
	{
		return shader;
	}

	//	Setters

	void	Mesh::setName(std::string name)
	{
		this->name = name;
	}

	void	Mesh::setColor(mft::vec3 color)
	{
		this->color = color;
	}

	void 	Mesh::setShader(unsigned int shader)
	{
		this->shader = shader;
	}

	void 	Mesh::setShader(GLShaderProgram* shader)
	{
		this->shader = shader->programID;
	}

	void 	Mesh::setAnimMatrix(const mft::mat4& mat)
	{
		this->animMatrix = mat;
		this->animDirty = true;
	}

	void	Mesh::draw(const Transform& parentGlobalTransform, const mft::mat4& parentGlobalMat, unsigned int overrideShader)
	{
		unsigned int finalShader = overrideShader == 0 ? this->shader : overrideShader;
		//	Recompute transform matrix if parent's one has changed
		if (parentTransform != this->parentTransform
			|| parentGlobalMat != this->parentMatrix
			|| this->transform.isDirty() || this->animDirty)
		{
			this->transform.getMatrix();
			parentGlobalTransform.getMatrix();
			this->globalMatrix =
				parentGlobalTransform.getPosMatrix() * this->transform.getPosMatrix()
				* parentGlobalTransform.getRotationMatrix() * this->transform.getRotationMatrix()
				* parentGlobalTransform.getScaleMatrix() * this->transform.getScaleMatrix();
			mft::vec3 parentInvScale = mft::vec3(1.0f) / parentGlobalTransform.getScale();
			mft::mat4 invRot = mft::mat4();
			invRot *= invRot.scale(parentInvScale);
			mft::mat4 trans = invRot * this->transform.getPosMatrix();
			std::cout << "Parent scale = " << parentGlobalTransform.getScale() << std::endl;
			std::cout << "Parent inv scale = " << parentInvScale << std::endl;
			std::cout << "Parent inv scale matrix = " << invRot << std::endl;
			trans *= this->transform.getRotationMatrix();

			trans *= this->transform.getScaleMatrix();
			trans *= parentGlobalTransform.getScaleMatrix();
			//trans.translate(this->transform.getScale());
			this->globalMatrix = parentGlobalMat * trans;
			//this->globalMatrix = parentGlobalMat * this->transform.getMatrix();
			if (this->name == "Mesh" || this->name == "Mesh 2")
			{
				std::cout << this->name << ":" << std::endl;
				std::cout << "Pos = " << this->transform.getPos() << std::endl;
				std::cout << "mult = localMatrix:" << this->transform.getMatrix() << " * parent global matrix" << parentTransform.getMatrix() << std::endl;
				std::cout << "= " << this->globalMatrix << std::endl << std::endl;
			}
			mft::mat4 tmp = mft::mat4::transpose(mft::mat4::inverse(this->globalMatrix));
			this->normalMatrix = mft::mat3(
				{ tmp[0][0], tmp[0][1], tmp[0][2] },
				{ tmp[1][0], tmp[1][1], tmp[1][2] },
				{ tmp[2][0], tmp[2][1], tmp[2][2] }
				);
			this->parentMatrix = parentGlobalMat;
			this->parentTransform = parentGlobalTransform;
			this->animDirty = false;
		}
		//std::cout << parentMat << " * " << matrix << " = " << tmp << std::endl;
		GLCallThrow(glUseProgram, finalShader);
		GLint location = GLCallThrow(glGetUniformLocation, finalShader, "baseColor");
		GLCallThrow(glUniform3f, location, color.x, color.y, color.z);
		//std::cout << "Drawing mesh " << name << " with matrix " << transformMatrix << std::endl;
		glMesh->draw(finalShader, this->globalMatrix, normalMatrix);
		for (auto child: children)
		{
			child->draw(this->transform, this->globalMatrix, finalShader);
		}
	}

	void	Mesh::addTexture(std::shared_ptr < Texture >& text)
	{
		(*glMesh).addTexture(text);
	}

	void	Mesh::addMesh(std::shared_ptr<Mesh> mesh)
	{
		children.push_back(mesh);
	}

	std::ostream& operator<<(std::ostream& o, Mesh const& mesh)
	{
		return o;
	}
}
