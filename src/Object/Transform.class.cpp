
#include "Object/Transform.class.hpp"

namespace notrealengine
{
	Transform::Transform( void ):
		tPos(), tRotation(), tScale(1),
		pos_mat(), rotation_mat(), scale_mat(),
		dirty_flags(DIRTY_FLAG_NONE)
	{
	}

	Transform::Transform( const Transform & other ):
		tPos(other.tPos), tRotation(other.tRotation), tScale(other.tScale),
		pos_mat(), rotation_mat(), scale_mat(),
		dirty_flags(other.dirty_flags)
	{
		// Copy over non-dirty cached matrices
		if (!(dirty_flags | DIRTY_FLAG_POS))
			pos_mat = other.pos_mat;
		if (!(dirty_flags | DIRTY_FLAG_ROTATION))
			rotation_mat = other.rotation_mat;
		if (!(dirty_flags | DIRTY_FLAG_SCALE))
			scale_mat = other.scale_mat;
	}

	Transform::Transform( vec3 pos, quat rotation, vec3 scale ):
		tPos(pos), tRotation(rotation), tScale(scale),
		pos_mat(), rotation_mat(), scale_mat(),
		dirty_flags(DIRTY_FLAG_ANY)
	{
	}

	Transform::~Transform( void )
	{
	}

	const vec3 & Transform::getPos( void ) const
	{
		return tPos;
	}

	const quat & Transform::getRotation( void ) const
	{
		return tRotation;
	}

	const vec3 & Transform::getScale( void ) const
	{
		return tScale;
	}

	const mat4 & Transform::getMatrix( void ) const
	{
		if (dirty_flags & DIRTY_FLAG_ANY)
			transform_mat = getScaleMatrix() * getRotationMatrix() * getPosMatrix();
		return transform_mat;
	}

	void Transform::setPos( vec3 new_pos )
	{
		if (tPos != new_pos)
		{
			dirty_flags |= DIRTY_FLAG_POS;
			tPos = new_pos;
		}
	}

	void Transform::setRotation( quat new_rotation )
	{
		if (tRotation != new_rotation)
		{
			dirty_flags |= DIRTY_FLAG_ROTATION;
			tRotation = new_rotation;
		}
	}

	void Transform::setScale( vec3 new_scale )
	{
		if (tScale != new_scale)
		{
			dirty_flags |= DIRTY_FLAG_SCALE;
			tScale = new_scale;
		}
	}

	void Transform::move( vec3 m )
	{
		setPos(tPos + m);
	}

	void Transform::rotate( quat r )
	{
		setRotation(tRotation * r);
	}

	void Transform::scale( vec3 s )
	{
		setScale(tScale * s);
	}

	const mat4 & Transform::getPosMatrix( void ) const
	{
		if (dirty_flags & DIRTY_FLAG_POS)
		{
			pos_mat = mat4::translate(tPos);
			dirty_flags &= ~DIRTY_FLAG_POS;
		}
		return pos_mat;
	}

	const mat4 & Transform::getRotationMatrix( void ) const
	{
		if (dirty_flags & DIRTY_FLAG_ROTATION)
		{
			rotation_mat = mat4::rotate(tRotation);
			dirty_flags &= ~DIRTY_FLAG_ROTATION;
		}
		return rotation_mat;
	}

	const mat4 & Transform::getScaleMatrix( void ) const
	{
		if (dirty_flags & DIRTY_FLAG_SCALE)
		{
			scale_mat = mat4::scale(tScale);
			dirty_flags &= ~DIRTY_FLAG_SCALE;
		}
		return scale_mat;
	}
}
