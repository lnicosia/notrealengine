#ifndef _GLOBJECT_CLASS_H_
# define _GLOBJECT_CLASS_H_

#include "Object/Mesh.class.hpp"
#include "Object/Texture.class.hpp"
#include "Object/Transform.class.hpp"
#include "Object/Asset.class.hpp"
#include "Object/Bone.class.hpp"
#include "Object/Animation.class.hpp"
#include "Object/ObjectImporter.class.hpp"

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

			/**	Update bones matrices with the actual bones state
			*/
			void
				bindBones( void ) const;

			/**	Set all the bones of the objet to their original state (no anim)
			*/
			void
				resetPose( void );

			/**	Set the object to a given keyframe of its current animation
			*/
			void
				setToKeyFrame(unsigned int keyFrame);

			/**	Launch a given animation for this object
			*/
			void
				playAnimation(Animation* anim,
					AnimationRepeat	animationRepeat = AnimationRepeat::Repeat);

			/**	Pause the current object's animation
			*/
			void
				pauseAnimation( void );

			/**	Resume the current object's animation
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

			//	All the bones of the object. Named BoneInfo to differenciate from
			//	animations' bones
			std::map<std::string, BoneInfo>	bones;
			//	Save the whole mesh hierarchy as a map too to access it
			//	in an easier way when applying a solid animation
			std::map<std::string, std::shared_ptr<Mesh>> meshesMap;
			int	nbBones;

			unsigned int	shader;

			GLenum	polygonMode;

			void
				loadObject(const std::string& path, unsigned int flags = 0);

			//	Animations

			Animation*		anim;
			float			startTime;
			float			pauseTime;
			AnimationState	animationState;

			/**	Update the current animation of the object according to
			**	the program's time
			*/
			void
				updateAnim( void );

			/**	Update a skeletal animation according to currentTime
			*/
			void
				updateSkeletalAnim(float currentTime);

			/**	Update a solid animation according to currentTime
			*/
			void
				updateSolidAnim(float currentTime);

			/**	Save the whole mesh hierarchy as a map too to access it
			**	in an easier way when applying a solid animation
			*/
			void
				BuildMeshesMap();

			/**
			*/
			void
				SaveMeshInMap(const std::shared_ptr<Mesh>& mesh);
	};

	std::ostream& operator<<(std::ostream& o, GLObject const& obj);
}

#endif
