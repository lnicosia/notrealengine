#include "mft/mft.hpp"

namespace mft
{
	float	rsqrt(float nb)
	{
		unsigned int		i;
		float		x2, y;

		x2 = nb * 0.5f;
		y = nb;
		unsigned int* p1 = reinterpret_cast<unsigned int*>(&nb);
		i = 0x5f3759df - (*p1 >> 1);
		float* p2 = reinterpret_cast<float*>(&i);
		float tmp = *p2;
		y = tmp * (1.5f - (x2 * tmp * tmp));
		return y;
	}

	template<int l, typename T>
	vec<l, T>	rsqrt(vec<l, T> const& v)
	{
		vec<l, float>	x2(v * 0.5f);
		vec<l, float>	y(v);
		vec<l, unsigned int>* p1 = reinterpret_cast<vec<l, unsigned int>*>(const_cast<vec<l, float>*>(&v));
		vec<l, unsigned int>	i = (unsigned int)0x5f3759df - (*p1 >> (unsigned int)1);
		vec<l, float>* p2 = reinterpret_cast<vec<l, float>*>(&i);
		vec<l, float>	tmp = *p2;
		vec<l, float>	res = tmp * (1.5f - (x2 * tmp * tmp));
		return res;
	}

	template<typename T>
	T	dot(vec<3, T> const& v1, vec<3, T> const& v2)
	{
		return (v1.x * v2.x + v1.y * v2.y + v1.z * v2.z);
	}


	template<typename T>
	T	dot(vec<4, T> const& v1, vec<4, T> const& v2)
	{
		return (v1.x * v2.x + v1.y * v2.y + v1.z * v2.z + v1.w * v2.w);
	}

	//	Normalized vector = v / |v|
	//	wich is also v * 1 / |v|
	//	and |v| is sqrt(v.x * v.x + ... + v.n * v.n)
	//	which is sqrt(dot(v, v))
	//	that gives us v * 1 / sqrt(v * v)
	//	ie v * rsqrt(v * v)
	template <int l, typename T>
	vec<l, T>	normalize(vec<l, T> const & v)
	{
		return (v * rsqrt(dot(v, v)));
	}
}