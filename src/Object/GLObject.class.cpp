#include "Object/GLObject.class.hpp"
#include "Object/TextureLoader.class.hpp"
#include "Object/AssetManager.class.hpp"
#include "Object/AssimpHelpers.hpp"
#include "GLContext.class.hpp"
#include "mft/mft.hpp"
#include "SDL.h"

//	OpenGL includes
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
#include <windows.h>
#include <glad/glad.h>
#else
# define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>
#endif

#include <fstream>
#include <algorithm>

namespace notrealengine
{

	GLObject::~GLObject()
	{
	}

	GLObject::GLObject(GLObject const& ref): Asset(ref.getPaths())
	{
		*this = ref;
	}

	GLObject::GLObject(const std::string& path)
		: Asset({path}),
		transform(), polygonMode(GL_FILL),
		directory(""), meshes(), bones(), nbBones(0),
		shader(GLContext::getShader("default")->programID),
		visible(true), max(0.0f), min(0.0f), isRangeInit(false),
		anim(nullptr), startTime(0.0f), pauseTime(0.0f),
		animationState(AnimationState::Stopped),
		animationRepeat(AnimationRepeat::Repeat)
	{
		loadObject(path);
		bindBones();
	}

	GLObject::GLObject(std::vector<std::shared_ptr<Mesh>>& meshes)
		: Asset({std::filesystem::path()}),
		transform(), polygonMode(GL_FILL),
		directory(""), meshes(meshes), bones(), nbBones(0),
		shader(GLContext::getShader("default")->programID),
		visible(true), max(0.0f), min(0.0f), isRangeInit(false),
		anim(nullptr), startTime(0.0f), pauseTime(0.0f),
		animationState(AnimationState::Stopped),
		animationRepeat(AnimationRepeat::Repeat)
	{

	}

	GLObject& GLObject::operator=(GLObject const& ref)
	{
		// TODO
		return *this;
	}

	//	Texture utility

	void	GLObject::addTexture(unsigned int mesh, std::shared_ptr < Texture >& text)
	{
		if (mesh >= 0 && mesh < meshes.size())
			(*meshes[mesh]).addTexture(text);
		else
			throw std::runtime_error("Mesh index out of bounds");
	}

	std::vector<std::shared_ptr<Texture>>	GLObject::loadMaterialTextures(aiMaterial* mat,
		aiTextureType type, std::string typeName, const aiScene *scene)
	{
		std::vector<std::shared_ptr<Texture>>	textures;

		for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
		{
			aiString	str;
			mat->GetTexture(type, i, &str);
			std::filesystem::path textPath(str.C_Str());
			textPath.make_preferred();
			std::string	path = directory + '/' + textPath.string();
			const aiTexture* texture;
			if ((texture = scene->GetEmbeddedTexture(str.C_Str())))
			{
				if (texture->mHeight == 0)
					textures.push_back(AssetManager::getInstance().loadAsset<Texture>(this->paths[0].string(),
						reinterpret_cast<unsigned char*>(texture->pcData),
						texture->mWidth, typeName));
				else
						textures.push_back(AssetManager::getInstance().loadAsset<Texture>(this->paths[0].string(),
						reinterpret_cast<unsigned char *>(texture->pcData),
						texture->mWidth * texture->mHeight, typeName));
			}
			else
			{
				//std::cout << "Loading " << typeName << " " << str.C_Str() << " from material" << std::endl;
				textures.push_back(AssetManager::getInstance().loadAsset<Texture>(path, typeName));
			}
		}
		return textures;
	}

	void	GLObject::SetVertexBoneData(Vertex& vertex, int id, float weight)
	{
		for (int i = 0; i < MAX_BONE_INFLUENCE; i++)
		{
			if (vertex.boneIDs[i] == -1)
			{
				vertex.boneIDs[i] = id;
				vertex.weights[i] = weight;
				//std::cout << "COUCOU" << std::endl;
				break;
			}
			//std::cout << "\tid = " << vertex.boneIDs[i] << ", weight = ";
			//std::cout << vertex.weights[i] << std::endl;
		}
	}

	void	GLObject::ExtractBoneInfo(std::vector<Vertex>& vertices, aiMesh* mesh,
		const aiScene* scene)
	{
		//std::cout << "Mesh " << mesh->mName.C_Str() << " has " << mesh->mNumBones << " bones" << std::endl;
		for (int i = 0; i < mesh->mNumBones; i++)
		{
			BoneInfo	bone;
			bone.id = -1;
			std::string	boneName = mesh->mBones[i]->mName.C_Str();
			bone.name = boneName;
			if (bones.find(boneName) == bones.end())
			{
				bone.id = nbBones;
				bone.offsetMatrix = AssimpToMftMatrix(mesh->mBones[i]->mOffsetMatrix);
				bone.modelMatrix = mft::mat4::inverse(bone.offsetMatrix);
				bone.localMatrix = mft::mat4();
				bone.fromParentMatrix = mft::mat4();
				bones[boneName] = bone;
				nbBones++;
			}
			else
			{
				bone.id = bones[boneName].id;
			}
			//std::cout << "Bone " << boneName <<  " id = " << bone.id << std::endl;
			aiVertexWeight *weights = mesh->mBones[i]->mWeights;
			int	nbWeights = mesh->mBones[i]->mNumWeights;
			for (int j = 0; j < nbWeights; j++)
			{
				if (weights[j].mVertexId >= vertices.size()
					|| weights[j].mWeight == 0)
					continue;
				//std::cout << "Vertex { " << weights[j].mVertexId << " } has the following bones:" << std::endl;
				SetVertexBoneData(vertices[weights[j].mVertexId], bone.id, weights[j].mWeight);
			}
		}
	}

	std::shared_ptr<Mesh>	GLObject::processMesh(aiMesh* mesh, const aiScene* scene)
	{
		std::vector<Vertex>						vertices;
		std::vector<unsigned int>				indices;
		std::vector<std::shared_ptr<Texture>>	textures;

		//	Vertices

		for (unsigned int i = 0; i < mesh->mNumVertices; i++)
		{
			Vertex	vertex;

			mft::vec3	vector;
			vector.x = mesh->mVertices[i].x;
			vector.y = mesh->mVertices[i].y;
			vector.z = mesh->mVertices[i].z;

			//	Get the range of each axis coordinate
			//	to scale the object to fit in our engine unit scale
			//	after parsing

			if (isRangeInit == false)
			{
				isRangeInit = true;
				max = vector;
				min = vector;
			}

			if (vector.x > max.x)
				max.x = vector.x;
			if (vector.y > max.y)
				max.y = vector.y;
			if (vector.z > max.z)
				max.z = vector.z;

			if (vector.x < min.x)
				min.x = vector.x;
			if (vector.y < min.y)
				min.y = vector.y;
			if (vector.z < min.z)
				min.z = vector.z;
			vertex.pos = vector;

			if (mesh->mNormals)
			{
				vector.x = mesh->mNormals[i].x;
				vector.y = mesh->mNormals[i].y;
				vector.z = mesh->mNormals[i].z;
				vertex.norm = vector;
			}
			else
			{
				vertex.norm = mft::vec3();
			}
			if (mesh->mTextureCoords[0] != NULL)
			{
				vertex.uv.x = mesh->mTextureCoords[0][i].x;
				vertex.uv.y = mesh->mTextureCoords[0][i].y;
			}
			else
			{
				vertex.uv = mft::vec2();
			}
			vertices.push_back(vertex);
		}
		ExtractBoneInfo(vertices, mesh, scene);

		//	Indices

		for (unsigned int i = 0; i < mesh->mNumFaces; i++)
		{
			aiFace	face = mesh->mFaces[i];
			for (unsigned int j = 0; j < face.mNumIndices; j++)
			{
				indices.push_back(face.mIndices[j]);
			}
		}

		if (mesh->mMaterialIndex >= 0)
		{
			aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
			std::vector<std::shared_ptr<Texture>> diffuseMaps = loadMaterialTextures(material,
				aiTextureType_DIFFUSE, "texture_diffuse", scene);
			textures.insert(
				textures.end(),
				std::make_move_iterator(diffuseMaps.begin()),
				std::make_move_iterator(diffuseMaps.end()));
			std::vector< std::shared_ptr<Texture>> specularMaps = loadMaterialTextures(material,
				aiTextureType_SPECULAR, "texture_specular", scene);
			textures.insert(
				textures.end(),
				std::make_move_iterator(specularMaps.begin()),
				std::make_move_iterator(specularMaps.end()));
		}

		MeshData	data = MeshData(vertices, indices);
		std::shared_ptr<GLMesh>	glMesh(new GLMesh(data, textures));
		std::shared_ptr<Mesh>	res(new Mesh(glMesh));
		res->setName(mesh->mName.C_Str());
		return res;
	}

	void	GLObject::processNodeBones(aiNode* node, const aiScene* scene, const mft::mat4& parentMat)
	{
		mft::mat4	transform = AssimpToMftMatrix(node->mTransformation) * parentMat;
		//std::cout << "Node '" << node->mName.C_Str() << "':" << std::endl;
		std::string name(node->mName.data);
		if (bones.contains(name))
		{
			bones[name].fromParentMatrix = transform;

			//std::cout << "Node " << node->mName.C_Str() << " has an associated bone" << std::endl;
			//std::cout << "Model matrix = " << bones[name].modelMatrix << std::endl;
			//std::cout << "Computed matrix = " << transform << std::endl;
			//std::cout << "Local matrix = " << bones[name].localMatrix << std::endl;
			//std::cout << "Transform - model = " << transform - bones[name].modelMatrix << std::endl;
			//std::cout << "Model - transform = " << bones[name].modelMatrix - transform << std::endl;
			bones[name].localMatrix = bones[name].offsetMatrix * transform;
		}
		for (unsigned int i = 0; i < node->mNumChildren; i++)
		{
			//std::cout << "Matrix sent to next node = " << transform << std::endl;
			processNodeBones(node->mChildren[i], scene, transform);
		}
	}

	void	GLObject::processNode(aiNode* node, const aiScene* scene)
	{
		//std::cout << "Node " << node->mName.C_Str() << std::endl;
		for (unsigned int i = 0; i < node->mNumMeshes; i++)
		{
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			// OPTI !! Multiple nodes can refer to the same mesh
			meshes.push_back(processMesh(mesh, scene));
		}
		for (unsigned int i = 0; i < node->mNumChildren; i++)
		{
			processNode(node->mChildren[i], scene);
		}
	}

	void	GLObject::loadObject(std::string path)
	{
		std::cout << "Loading object '" << path << "'..." << std::endl;

		Assimp::Importer	importer;
		const aiScene* scene;
		if (path.substr(path.find_last_of('.')) == ".dae")
			scene = importer.ReadFile(path, aiProcess_Triangulate
				| aiProcess_GenUVCoords);
		else
			scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs
				| aiProcess_GenUVCoords);

		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		{
			std::cerr << "assimp: " << importer.GetErrorString() << std::endl;
			return;
		}
		directory = path.substr(0, path.find_last_of('/'));
		processNode(scene->mRootNode, scene);
		processNodeBones(scene->mRootNode, scene, mft::mat4());

		//	Scale the object

		float rangeX = this->max.x - this->min.x;
		float rangeY = this->max.y - this->min.y;
		float rangeZ = this->max.z - this->min.z;

		float minRange;
		if (rangeX == 0)
			minRange = std::min(rangeY, rangeZ);
		else if (rangeY == 0)
			minRange = std::min(rangeZ, rangeX);
		else if (rangeZ == 0)
			minRange = std::min(rangeY, rangeX);
		else
			minRange = std::min(std::min(rangeX, rangeY), rangeZ);

		float scale = 2.0f / minRange;
		this->transform.scale(mft::vec3(scale));
	}

	//	Drawing functions

	void	GLObject::draw( void )
	{
		if (this->animationState == AnimationState::Playing)
			this->updateAnim();
		for (size_t i = 0; i < meshes.size(); i++)
		{
			meshes[i]->draw(transform.getMatrix(), this->shader);
		}
	}

	void	GLObject::drawBones( void ) const
	{
		unsigned int shader = GLContext::getShader("colorUnlit")->programID;
		GLCallThrow(glUseProgram, shader);
		GLCallThrow(glDisable, GL_DEPTH_TEST);
		std::map<std::string, BoneInfo>::const_iterator it;
		Mesh	cube(GLContext::cube);
		cube.setColor(mft::vec3(204.0f / 255.0f, 0.0f, 204.0f / 255.0f));
		cube.setShader(shader);
		for (it = bones.begin(); it != bones.end(); it++)
		{
			cube.draw((*it).second.fromParentMatrix * transform.getMatrix());
		}
		cube.setColor(mft::vec3(0.0f, 1.0f, 0.0f));
		for (it = bones.begin(); it != bones.end(); it++)
		{
			cube.draw((*it).second.modelMatrix * transform.getMatrix());
		}
		GLCallThrow(glEnable, GL_DEPTH_TEST);
	}

	void	GLObject::bindBones( void ) const
	{
		unsigned int shader = this->shader == 0 ? GLContext::getShader("default")->programID : this->shader;
		GLCallThrow(glUseProgram, shader);
		GLint location;
		std::string str;
		std::map<std::string, BoneInfo>::const_iterator it;
		for (it = bones.begin(); it != bones.end(); it++)
		{
			str = "bonesMatrices[" + std::to_string(it->second.id) + "]";
			location = GLCallThrow(glGetUniformLocation, shader, str.c_str());
			GLCallThrow(glUniformMatrix4fv, location, 1, GL_TRUE,
				static_cast<const float*>(it->second.localMatrix));
		}
	}

	void	GLObject::resetPose()
	{
		unsigned int shader = this->shader == 0 ? GLContext::getShader("default")->programID : this->shader;
		GLCallThrow(glUseProgram, shader);
		GLint location;
		mft::mat4	mat = mft::mat4();
		std::string str;
		int i = 0;
		std::map<std::string, BoneInfo>::iterator it;
		for (it = bones.begin(); it != bones.end(); it++)
		{
			str = "bonesMatrices[" + std::to_string(i) + "]";
			location = GLCallThrow(glGetUniformLocation, shader, str.c_str());
			GLCallThrow(glUniformMatrix4fv, location, 1, GL_TRUE,
				static_cast<const float*>(mat));
			it->second.localMatrix = mat;
			i++;
		}
		this->animationState = AnimationState::Stopped;
	}

	void	GLObject::pauseAnimation( void )
	{
		if (this->animationState != AnimationState::Playing)
			return;
		this->pauseTime = static_cast<float>(SDL_GetTicks());
		this->animationState = AnimationState::Paused;
	}

	void	GLObject::resumeAnimation( void )
	{
		if (this->animationState != AnimationState::Paused)
			return;
		this->startTime += static_cast<float>(SDL_GetTicks()) - this->pauseTime;
		this->animationState = AnimationState::Playing;
	}

	void	GLObject::updateAnim( void )
	{
		if (this->anim == nullptr)
		{
			this->resetPose();
			return;
		}
		float currentTime = static_cast<float>(SDL_GetTicks()) - this->startTime;
		if (currentTime >= anim->getDuration())
		{
			this->animationState = AnimationState::Stopped;
			if (this->animationRepeat == AnimationRepeat::Repeat)
				playAnimation(anim);
			else if (this->animationRepeat == AnimationRepeat::ResetPose)
				resetPose();
			return;
		}
		std::map<std::string, Bone> animBones = anim->getBones();
		std::vector<AnimNode>& animNodes = anim->getNodes();
		for (unsigned int i = 0; i < animNodes.size(); i++)
		{
			AnimNode& node = animNodes[i];

			//	If a bone of the animation is associated with this node,
			//	use its animation transform

			if (animBones.contains(node.name))
			{
				node.transform = animBones[node.name].getTransform(currentTime)
					* animNodes[node.parentId].transform;
			}
			//	Otherwise, use the original node's transform
			else
			{
				node.transform = node.transform
					* animNodes[node.parentId].transform;
			}
			//	If a bone of the object is associated with this node,
			//	update its transform with what we just computed
			if (this->bones.contains(node.name))
			{
				BoneInfo& bone = this->bones[node.name];

				bone.modelMatrix = node.transform;
				bone.localMatrix = bone.offsetMatrix * bone.modelMatrix;
			}
		}
		this->bindBones();
	}

	void	GLObject::setToKeyFrame(unsigned int keyFrame)
	{
		if (this->anim == nullptr)
			return ;
		std::map<std::string, Bone> animBones = anim->getBones();
		std::vector<AnimNode>& animNodes = anim->getNodes();
		for (unsigned int i = 0; i < animNodes.size(); i++)
		{
			AnimNode& node = animNodes[i];

			//if (keyFrame >= animBones[node.name].getNbTransforms())
			//	continue ;
			//	If a bone of the animation is associated with this node,
			//	use its animation transform

			if (animBones.contains(node.name))
			{
				node.transform = animBones[node.name].getTransform(keyFrame)
					* animNodes[node.parentId].transform;
			}
			//	Otherwise, use the original node's transform
			else
			{
				node.transform = node.transform
					* animNodes[node.parentId].transform;
			}
			//	If a bone of the object is associated with this node,
			//	update its transform with what we just computed
			if (this->bones.contains(node.name))
			{
				BoneInfo& bone = this->bones[node.name];

				bone.modelMatrix = node.transform;
				bone.localMatrix = bone.offsetMatrix * bone.modelMatrix;
			}
		}
		this->bindBones();
	}

	void	GLObject::playAnimation(Animation* anim, AnimationRepeat animationRepeat)
	{
		if (anim == nullptr)
			return;
		this->anim = anim;
		this->animationState = AnimationState::Playing;
		this->animationRepeat = animationRepeat;
		this->startTime = static_cast<float>(SDL_GetTicks());
	}

	//	Accessors

	const std::vector<std::shared_ptr<Mesh>>&	GLObject::getMeshes() const
	{
		return meshes;
	}

	std::map<std::string, BoneInfo>&	GLObject::getBones()
	{
		return bones;
	}

	const int GLObject::getNbBones() const
	{
		return nbBones;
	}

	const std::string GLObject::getAssetType() const
	{
		return std::string("GLObject");
	}

	const unsigned int GLObject::getShader() const
	{
		return shader;
	}

	const AnimationState& GLObject::getAnimationState() const
	{
		return animationState;
	}

	//	Setters

	void	GLObject::setName(std::string name)
	{
		this->name = name;
	}

	void GLObject::setShader(unsigned int shader)
	{
		this->shader = shader;
	}

	void GLObject::setShader(GLShaderProgram *shader)
	{
		this->shader = shader->programID;
	}

	void GLObject::setAnimation(Animation* anim)
	{
		this->anim = anim;
		this->resetPose();
	}

	std::ostream& operator<<(std::ostream& o, GLObject const& obj)
	{
		std::vector<std::shared_ptr<Mesh>>	meshes = obj.getMeshes();
		std::cout << obj.getName();
		for (size_t i = 0; i < meshes.size(); i++)
		{
			std::cout << "Mesh " << i << ":" << std::endl << *meshes[i];
		}
		return o;
	}
}
