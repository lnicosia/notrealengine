
#ifndef  __TRANSFORM_CLASS_H__
# define __TRANSFORM_CLASS_H__

# include "mft/mft.hpp"

namespace notrealengine
{
	using namespace mft;

	class Transform
	{

	public:
		/** *structors **/
		Transform( void );
		Transform( const Transform & other );
		Transform( vec3 pos, quat rotation, vec3 scale );

		Transform &
			operator=( const Transform & other );
		bool
			operator==( const Transform & other ) const ;

		~Transform( void );

		/** Accessors **/
		const vec3 &
			getPos( void ) const;
		const vec3 &
			getScale( void ) const;
		const quat &
			getRotation( void ) const;

		const mat4 &
			getMatrix( void ) const;
		const bool
			isDirty( void ) const;

		void setPos( vec3 new_pos );
		void setRotation( quat new_rotation );
		void setScale( vec3 new_scale );

		void move( vec3 translation );
		void rotate( quat rotation );
		void scale( vec3 magnitude );

		const mat4 &
			getPosMatrix( void ) const;
		const mat4 &
			getRotationMatrix( void ) const;
		const mat4 &
			getScaleMatrix( void ) const;

		/** Others **/

	private:
		vec3 tPos;
		quat tRotation;
		vec3 tScale;

		// Used for caching, along with the dirty flags
		mutable mat4
			pos_mat,
			rotation_mat,
			scale_mat,
			transform_mat;

		enum {
			DIRTY_FLAG_NONE = 0,
			DIRTY_FLAG_POS = 1 << 0,
			DIRTY_FLAG_ROTATION = 1 << 1,
			DIRTY_FLAG_SCALE = 1 << 2,
			DIRTY_FLAG_ANY = DIRTY_FLAG_POS | DIRTY_FLAG_ROTATION | DIRTY_FLAG_SCALE
		};

		mutable int dirty_flags;

	};
}

#endif
