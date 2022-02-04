#ifndef _GLOBJECT_CLASS_H_
# define _GLOBJECT_CLASS_H_

#include "Object/Mesh.class.hpp"
#include "Object/Texture.class.hpp"
#include "Object/Transform.class.hpp"
#include "Object/Asset.class.hpp"
#include "Object/Bone.class.hpp"
#include "Object/Animation.class.hpp"

//	Fix for assimp
#undef max
#undef min
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

#include <memory>
#include <map>

enum AnimationState
{
	Playing,
	Paused,
	Stopped
};

enum AnimationRepeat
{
	Repeat,
	Stop,
	ResetPose
};

namespace notrealengine
{
	struct BoneInfo
	{
		int	id;
		std::string	name;
		mft::mat4	offsetMatrix;
		mft::mat4	localMatrix;
		mft::mat4	modelMatrix;
		mft::mat4	fromParentMatrix;
	};

	class GLObject: public Asset
	{

		public:

			GLObject() = delete;
			GLObject(GLObject const & GLObject);
			GLObject(const std::string& path);
			GLObject(std::vector<std::shared_ptr<Mesh>>& meshes);
			~GLObject();
			GLObject& operator=(GLObject const& GLObject);

			//	Draw functions

			void
				draw( void );

			/**	Render all the object's bones
			*/
			void
				drawBones( void ) const;

			/**
			*/
			void
				bindBones( void ) const;

			/**
			*/
			void
				resetPose( void );

			/**
			*/
			void
				setToKeyFrame(unsigned int keyFrame);

			/**
			*/
			void
				playAnimation(Animation* anim,
					AnimationRepeat	animationRepeat = AnimationRepeat::Repeat);
			/**
			*/
			void
				pauseAnimation( void );
			/**
			*/
			void
				resumeAnimation( void );

			//	Accessors

			const std::vector<std::shared_ptr<Mesh>>&
				getMeshes() const;
			std::map<std::string, BoneInfo>&
				getBones();
			const int
				getNbBones() const;
			const unsigned int
				getShader() const;
			const AnimationState&
				getAnimationState() const;

			virtual const std::string
				getAssetType() const;

			//	Setters

			void
				setName(std::string name);
			void
				setShader(unsigned int shader);
			void
				setShader(GLShaderProgram* shader);
			void
				setBoneGlobalMatrix(const mft::mat4& ref);
			void
				setBoneLocalMatrix(const mft::mat4& ref);
			void
				setAnimation(Animation* anim);

			//	Texture utility

			void	addTexture(unsigned int mesh, std::shared_ptr < Texture> & text);

			// Transform is public so its non-const operations can be called efficiently
			Transform	transform;

			bool	visible;

			AnimationRepeat	animationRepeat;

		private:

			mft::vec3	max;
			mft::vec3	min;
			bool		isRangeInit;

			std::vector<std::shared_ptr<Mesh>>	meshes;

			std::string	directory;

			std::map<std::string, BoneInfo>	bones;
			int	nbBones;

			unsigned int	shader;

			GLenum	polygonMode;

			//	Object loading
			void
				SetVertexBoneData(Vertex& vertex, int id, float weight);
			void
				ExtractBoneInfo(std::vector<Vertex>& vertices, aiMesh* mesh, const aiScene* scene);
			void
				loadObject(std::string path);
			void
				processNode(aiNode* node, const aiScene* scene);
			void
				processNodeBones(aiNode* node, const aiScene* scene, const mft::mat4& parentMat);
			std::shared_ptr<Mesh>
				processMesh(aiMesh* mesh, const aiScene* scene);
			std::vector<std::shared_ptr<Texture>>
				loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName, const aiScene *scene);
			/**	Sometimes bones are missing from the original skeleton but can be
			**	found in the object's animations
			*/
			void
				readMissingBones(const aiScene* scene);

			//	Animations

			Animation*		anim;
			float			startTime;
			float			pauseTime;
			AnimationState	animationState;

			/**
			*/
			void
				updateAnim();
	};

	std::ostream& operator<<(std::ostream& o, GLObject const& obj);
}

#endif
