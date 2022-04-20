#include "Camera.class.hpp"

namespace notrealengine
{
	Camera::Camera(mft::vec3 pos)
		: pos(pos), up(mft::vec3(0.0f, 1.0f, 0.0f)), front(mft::vec3(0.0f, 0.0f, -1.0f)),
		yaw(-90.0f), pitch(0.0f), speed(0.005f), sensitivity(0.25f)
	{
		update();
	}

	Camera::~Camera()
	{
	}

	void	Camera::update()
	{
		if (pitch >= 89.0f)
			pitch = 89.0f;
		if (pitch <= -89.0f)
			pitch = -89.0f;

		mft::vec3 Front;
		Front.x = cosf(mft::radians(yaw)) * cosf(mft::radians(pitch));
		Front.y = sinf(mft::radians(pitch));
		Front.z = sinf(mft::radians(yaw)) * cosf(mft::radians(pitch));
		front = mft::vec3::normalized(Front);

		view = mft::mat4::lookAt(pos, pos + front, up);
	}

	mft::mat4 const	Camera::getViewMatrix() const
	{
		return view;
	}

	//	Movements

	void	Camera::forward(uint32_t time)
	{
		pos -= (time * speed) * front;
		view = mft::mat4::lookAt(pos, pos + front, up);
	}

	void	Camera::backward(uint32_t time)
	{
		pos += (time * speed) * front;
		view = mft::mat4::lookAt(pos, pos + front, up);
	}

	void	Camera::left(uint32_t time)
	{
		pos -= mft::vec3::normalized(mft::vec3::cross(front, up)) * (time * speed);
		view = mft::mat4::lookAt(pos, pos + front, up);
	}

	void	Camera::right(uint32_t time)
	{
		pos += mft::vec3::normalized(mft::vec3::cross(front, up)) * (time * speed);
		view = mft::mat4::lookAt(pos, pos + front, up);
	}
}
