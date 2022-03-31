#include "Object/Mesh.class.hpp"
#include "GLContext.class.hpp"

namespace notrealengine
{
	Mesh::Mesh(std::shared_ptr<GLMesh> const& glMesh)
		: name(glMesh->getName()),
		localTransform(), animTransform(), finalLocalTransform(),
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

	void Mesh::updateFinalTransform( void )
	{
		this->finalLocalTransform.setPos(this->localTransform.getPos() + this->animTransform.getPos());
		this->finalLocalTransform.setRotation(this->localTransform.getRotation() * this->animTransform.getRotation());
		this->finalLocalTransform.setScale(this->localTransform.getScale() * this->animTransform.getScale());
	}

	void	Mesh::draw(const mft::vec3 globalScale, const mft::mat4& parentGlobalMat, unsigned int overrideShader)
	{
		unsigned int finalShader = overrideShader == 0 ? this->shader : overrideShader;
		static const float epsilon = std::numeric_limits<float>::epsilon();
		//	Recompute transforms if parent, local or anim transforms changed
		if (parentGlobalMat != this->parentMatrix || this->animTransform.isDirty()
			|| this->localTransform.isDirty())
		{
			this->localTransform.getMatrix();
			this->animTransform.getMatrix();
			this->finalLocalTransform.setPos(this->localTransform.getPos() + this->animTransform.getPos());
			this->finalLocalTransform.setRotation(this->localTransform.getRotation() * this->animTransform.getRotation());
			this->finalLocalTransform.setScale(this->localTransform.getScale() * this->animTransform.getScale());
			//	Specific handling for non uniform scales:
			//	unscale the current mesh before applying rotation
			//	then rescale
			if (std::fabs(globalScale.x - globalScale.y) > epsilon
				|| std::fabs(globalScale.x - globalScale.z) > epsilon)
			{
				//	Get 1 / parentScale to unscale
				mft::vec3 globalInvScale = mft::vec3(1.0f) / globalScale;
				mft::mat4 invRot = mft::mat4::scale(globalInvScale);

				mft::mat4 trans = this->finalLocalTransform.getPosMatrix();
				trans *= invRot;
				trans *= this->finalLocalTransform.getRotationMatrix();
				trans *= this->finalLocalTransform.getScaleMatrix();
				trans *= mft::mat4::scale(globalScale);

				this->globalMatrix = parentGlobalMat * trans;
			}
			else
				this->globalMatrix = parentGlobalMat * this->finalLocalTransform.getMatrix();

			mft::mat4 tmp = mft::mat4::transpose(mft::mat4::inverse(this->globalMatrix));
			this->normalMatrix = mft::mat3(
				{ tmp[0][0], tmp[0][1], tmp[0][2] },
				{ tmp[1][0], tmp[1][1], tmp[1][2] },
				{ tmp[2][0], tmp[2][1], tmp[2][2] }
				);
			this->parentMatrix = parentGlobalMat;
			//this->parentTransform = parentGlobalTransform;
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
			child->draw(globalScale * this->finalLocalTransform.getScale(), this->globalMatrix, finalShader);
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
