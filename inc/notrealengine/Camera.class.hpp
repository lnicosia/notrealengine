#ifndef _CAMERA_CLASS_H_
# define _CAMERA_CLASS_H_

#include "mft/mft.hpp"

namespace notrealengine
{
	class Camera
	{
	public:
		Camera(mft::vec3 pos = mft::vec3(0.0f, 0.0f, 0.0f));
		~Camera();

		mft::mat4 const	getViewMatrix() const;

		mft::vec3	pos;
		mft::vec3	front;
		mft::vec3	up;

		float	yaw;
		float	pitch;
		//	We don't need roll for a first person camera

		float	speed;
		float	sensitivity;

		void	update();

		//	Movements

		void	forward(uint32_t time);
		void	backward(uint32_t time);
		void	left(uint32_t time);
		void	right(uint32_t time);

	private:

		mft::mat4	view;

	};
}

#endif
