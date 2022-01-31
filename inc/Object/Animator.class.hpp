#ifndef _ANIMATOR_CLASS_H_
#define _ANIMATOR_CLASS_H_

namespace notrealengine
{
	class Animator
	{
	public:
		Animator();
		~Animator();

	private:
		float	currentTime;
		float	deltaTime;
	};
}

#endif // !_ANIMATOR_CLASS_H_
