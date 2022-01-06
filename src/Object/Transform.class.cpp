
#include "Object/Transform.class.hpp"

namespace notrealengine
{
	Transform::Transform( void ):
		pos(), rotation(), scale(1),
		pos_mat(), rotation_mat(), scale_mat(),
		dirty_flags(DIRTY_FLAG_NONE)
	{
	}

	Transform::Transform( const Transform & other ):
		pos(other.pos), rotation(other.rotation), scale(other.scale),
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
		pos(pos), rotation(rotation), scale(scale),
		pos_mat(), rotation_mat(), scale_mat(),
		dirty_flags(DIRTY_FLAG_ANY)
	{
	}

	Transform::~Transform( void )
	{
	}

	const vec3 & Transform::getPos( void ) const
	{
		return pos;
	}

	const quat & Transform::getRotation( void ) const
	{
		return rotation;
	}

	const vec3 & Transform::getScale( void ) const
	{
		return scale;
	}

	const mat4 & Transform::getMatrix( void ) const
	{
		if (dirty_flags & DIRTY_FLAG_ANY)
			transform_mat = getScaleMatrix() * getRotationMatrix() * getPosMatrix();
		return transform_mat;
	}

	void Transform::setPos( vec3 new_pos )
	{
		if (pos != new_pos)
		{
			dirty_flags |= DIRTY_FLAG_POS;
			pos = new_pos;
		}
	}

	void Transform::setRotation( quat new_rotation )
	{
		if (rotation != new_rotation)
		{
			dirty_flags |= DIRTY_FLAG_ROTATION;
			rotation = new_rotation;
		}
	}

	void Transform::setScale( vec3 new_scale )
	{
		if (scale != new_scale)
		{
			dirty_flags |= DIRTY_FLAG_SCALE;
			scale = new_scale;
		}
	}

	void Transform::moved( vec3 moved )
	{
		setPos(pos + moved);
	}

	void Transform::rotated( quat rotated )
	{
		setRotation(rotation * rotated);
	}

	void Transform::scaled( vec3 scaled )
	{
		setScale(scale * scaled);
	}

	const mat4 & Transform::getPosMatrix( void ) const
	{
		if (dirty_flags & DIRTY_FLAG_POS)
			pos_mat = mat4::translate(pos);
		return pos_mat;
	}

	const mat4 & Transform::getRotationMatrix( void ) const
	{
		if (dirty_flags & DIRTY_FLAG_ROTATION)
			rotation_mat = mat4::rotate(rotation);
		return rotation_mat;
	}

	const mat4 & Transform::getScaleMatrix( void ) const
	{
		if (dirty_flags & DIRTY_FLAG_SCALE)
			scale_mat = mat4::scale(scale);
		return scale_mat;
	}
}
