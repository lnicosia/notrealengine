#include "Camera.class.hpp"

namespace notrealengine
{
	Camera::Camera(mft::vec3 pos)
		: pos(pos), up(mft::vec3(0.0f, 1.0f, 0.0f))
	{
	}

	Camera::~Camera()
	{
	}

	void	Camera::update()
	{

	}

	mft::mat4 const	Camera::getViewMatrix() const
	{
		return mft::mat4::lookAt(mft::vec3::normalized(pos),
			mft::vec3::normalized(pos + front),
			mft::vec3::normalized(up));
	}
}