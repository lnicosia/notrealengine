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

		void	update();

	private:
		float	vfov;
		float	hfov;
		float	ratio;
		float	left;
		float	right;
		float	bottom;
		float	top;
		float	nearZ;
		float	farZ;

		float	yaw;
		float	pitch;
		//	We don't need roll for a first person camera

		float	speed;

	};
}

#endif
