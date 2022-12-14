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
				playAnimation(std::shared_ptr<Animation> anim,
					AnimationRepeat	animationRepeat = AnimationRepeat::Repeat);

			/**	Pause the current object's animation
			*/
			void
				pauseAnimation( void );

			/**	Resume the current object's animation
			*/
			void
				resumeAnimation( void );

			/**	Stop the current object's animation
			*/
			void
				stopAnimation( void );


			//	Accessors

			const std::vector<std::shared_ptr<Mesh>>&
				getMeshes() const;
			std::map<std::string, BoneInfo>&
				getBones();
			const size_t
				getNbBones() const;
			const unsigned int
				getShader() const;
			const AnimationState&
				getAnimationState() const;
			const std::shared_ptr<Animation>
				getAnimation() const;
			std::map<std::string, std::shared_ptr<Mesh>>&
				getMeshesMap();
			/**	Returns the start time of the current anim
			*/
			const float
				getStartTime() const;
			/**	Returns the start time of the current anim
			*/
			const float
				getCurrentTime() const;
			/**	ToStrings the actual animation state (ie Playing, Paused, Stopped)
			*/
			const std::string
				getAnimationStateStr() const;

			//	Asset inheritence

			const std::string
				getAssetType() const override;

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
				setAnimation(std::shared_ptr<Animation> anim);

			//	Texture utility

			void
				addTexture(unsigned int mesh, std::shared_ptr < Texture> & text);

			// Transform is public so its non-const operations can be called efficiently
			Transform	transform;

			bool	visible;

			AnimationRepeat	animationRepeat;

			double	animationSpeed;

		private:

			std::vector<std::shared_ptr<Mesh>>	meshes;

			//	All the bones of the object. Named BoneInfo to differenciate from
			//	animations' bones
			std::map<std::string, BoneInfo>	bones;
			//	Save the whole mesh hierarchy as a map too to access it
			//	in an easier way when applying a solid animation
			std::map<std::string, std::shared_ptr<Mesh>> meshesMap;

			unsigned int	shader;

			GLenum	polygonMode;

			void
				loadObject(const std::string& path, unsigned int flags = 0);

			//	Animations

			std::shared_ptr<Animation>		anim;
			/**	Start time of the current animation (in ms)
			*/
			float			startTime;
			/**	Program time when the current animation is paused
			**	to be able to resume at the correct time
			*/
			float			pauseTime;
			/**	We do not need to store the current time value
			**	it's only for debugging purposes
			*/
			float			currentTime;
			AnimationState	animationState;

			/**	Update the current animation of the object according to
			**	the program's time
			*/
			void
				updateAnim( void );

			/**	Update a skeletal animation according to currentTime
			*/
			void
				updateSkeletalAnim( void );

			/**	Update a solid animation according to currentTime
			*/
			void
				updateSolidAnim( void );

			/**	Save the whole mesh hierarchy as a map too to access it
			**	in an easier way when applying a solid animation
			*/
			void
				BuildMeshesMap();

			/**	Recursive function to store the mesh hierarchy in one single map
			**	Called by BuildMeshesMap on the root mesh
			*/
			void
				SaveMeshInMap(const std::shared_ptr<Mesh>& mesh);
	};

	std::ostream& operator<<(std::ostream& o, GLObject const& obj);
}

#endif
