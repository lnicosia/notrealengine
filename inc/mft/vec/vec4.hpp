#ifndef _VEC4_H_
# define _VEC4_H_

#include <iostream>
#include "mft/vec.hpp"

namespace	mft
{
	template <typename T>
	struct	vec<4, T>
	{
		T	x;
		T	y;
		T	z;
		T	w;

		vec() : x(0), y(0), z(0)
		{
		}

		vec(vec<4, T> const& v) : x(v.x), y(v.y), z(v.z), w(v.w)
		{

		}

		vec(T x, T y, T z) : x(x), y(y), z(z), w(w)
		{

		}

		vec(T scalar) : x(scalar), y(scalar), z(scalar), w(scalar)
		{

		}

		vec(T x, T y, T z, T w) : x(x), y(y), z(z), w(w)
		{

		}


		T & operator[](int i)
		{
			if (!(i >= 0 && i < 4))
				return x;
			switch (i)
			{
			default:
			case 0:
				return x;
			case 1:
				return y;
			case 2:
				return z;
			case 3:
				return w;
			}
		}

		T const & operator[](int i) const
		{
			if (!(i >= 0 && i < 4))
				return x;
			switch (i)
			{
			default:
			case 0:
				return x;
			case 1:
				return y;
			case 2:
				return z;
			case 3:
				return w;
			}
		}

		template <typename U>
		vec<4, T> & operator=(vec<4, U> const& v)
		{
			x = static_cast<T>(v.x);
			y = static_cast<T>(v.y);
			z = static_cast<T>(v.z);
			w = static_cast<T>(v.w);
			return *this;
		}

		template <typename U>
		vec<4, T> & operator*=(U scalar)
		{
			x *= static_cast<T>(scalar);
			y *= static_cast<T>(scalar);
			z *= static_cast<T>(scalar);
			w *= static_cast<T>(scalar);
			return *this;
		}

		template <typename U>
		vec<4, T> & operator/=(U scalar)
		{
			x /= static_cast<T>(scalar);
			y /= static_cast<T>(scalar);
			z /= static_cast<T>(scalar);
			w /= static_cast<T>(scalar);
			return *this;
		}

		static int size()
		{
			return 4;
		}
	};

	/*template <typename T>
	template <typename X, typename Y, typename Z, typename W>
	vec<4, T>::vec(X x, Y y, Z z, W w) :
		x(static_cast<T>(x)),
		y(static_cast<T>(y)),
		z(static_cast<T>(z)),
		w(static_cast<T>(w))
	{

	}*/

	// Binary operators

	template <typename T>
	vec<4, T> operator+(vec<4, T> const& v, T scalar)
	{
		return vec<4, T>(
			v.x + scalar,
			v.y + scalar,
			v.z + scalar,
			v.w + scalar);
	}

	template <typename T>
	vec<4, T> operator+(T scalar, vec<4, T> const& v)
	{
		return vec<4, T>(
			scalar + v.x,
			scalar + v.y,
			scalar + v.z,
			scalar + v.w);
	}

	template <typename T>
	vec<4, T> operator+(vec<4, T> const& v1, vec<4, T> const& v2)
	{
		return vec<4, T>(
			v1.x + v2.x,
			v1.y + v2.y,
			v1.z + v2.z,
			v1.w + v2.w);
	}

	template <typename T>
	vec<4, T> operator-(vec<4, T> const& v, T scalar)
	{
		return vec<4, T>(
			v.x - scalar,
			v.y - scalar,
			v.z - scalar,
			v.w - scalar);
	}

	template <typename T>
	vec<4, T> operator-(T scalar, vec<4, T> const& v)
	{
		return vec<4, T>(
			scalar - v.x,
			scalar - v.y,
			scalar - v.z,
			scalar - v.w);
	}

	template <typename T>
	vec<4, T> operator-(vec<4, T> const& v1, vec<4, T> const& v2)
	{
		return vec<4, T>(
			v1.x - v2.x,
			v1.y - v2.y,
			v1.z - v2.z,
			v1.w + v2.w);
	}

	template <typename T>
	vec<4, T> operator*(vec<4, T> const& v, T scalar)
	{
		return vec<4, T>(
			v.x * scalar,
			v.y * scalar,
			v.z * scalar,
			v.w * scalar);
	}

	template <typename T>
	vec<4, T> operator*(T scalar, vec<4, T> const& v)
	{
		return vec<4, T>(
			scalar * v.x,
			scalar * v.y,
			scalar * v.z,
			scalar * v.w);
	}

	template <typename T>
	vec<4, T> operator*(vec<4, T> const& v1, vec<4, T> const& v2)
	{
		return vec<4, T>(
			v1.x * v2.x,
			v1.y * v2.y,
			v1.z * v2.z,
			v1.w * v2.w);
	}

	template <typename T>
	vec<4, T> operator/(vec<4, T> const& v, T scalar)
	{
		return vec<4, T>(
			v.x / scalar,
			v.y / scalar,
			v.z / scalar,
			v.w / scalar);
	}

	template <typename T>
	vec<4, T> operator/(T scalar, vec<4, T> const& v)
	{
		return vec<4, T>(
			scalar / v.x,
			scalar / v.y,
			scalar / v.z,
			scalar / v.w);
	}

	template <typename T>
	vec<4, T> operator/(vec<4, T> const& v1, vec<4, T> const& v2)
	{
		return vec<4, T>(
			v1.x / v2.x,
			v1.y / v2.y,
			v1.z / v2.z,
			v1.w / v2.w);
	}

	//	Bit operators, needed for fast rsqrt calculations

	template <typename T>
	vec<4, T> operator<<(vec<4, T> const& v1, T scalar)
	{
		return vec<4, T>(
			v1.x << scalar,
			v1.y << scalar,
			v1.z << scalar,
			v1.w << scalar);
	}

	template <typename T>
	vec<4, T> operator<<(T scalar, vec<4, T> const& v1)
	{
		return vec<4, T>(
			scalar << v1.x,
			scalar << v1.y,
			scalar << v1.z,
			scalar << v1.w);
	}

	template <typename T>
	vec<4, T> operator>>(vec<4, T> const& v1, T scalar)
	{
		return vec<4, T>(
			v1.x >> scalar,
			v1.y >> scalar,
			v1.z >> scalar,
			v1.w >> scalar);
	}

	template <typename T>
	vec<4, T> operator>>(T scalar, vec<4, T> const& v1)
	{
		return vec<4, T>(
			scalar >> v1.x,
			scalar >> v1.y,
			scalar >> v1.z,
			scalar >> v1.w);
	}

	template <typename T>
	bool operator==(vec<4, T> const& v1, vec<4, T> const& v2)
	{
		return (v1.x == v2.x
				&& v1.y == v2.y
				&& v1.z == v2.z
				&& v1.w == v2.w);
	}

	template <typename T>
	bool operator!=(vec<4, T> const& v1, vec<4, T> const& v2)
	{
		return (v1.x != v2.x
				|| v1.y != v2.y
				|| v1.z != v2.z
				|| v1.w != v2.w);
	}

	template <typename T>
	std::ostream& operator<<(std::ostream& o, vec<4, T> const& v)
	{
		std::cout << "{ " << v.x << " " << v.y << " " << v.z << " " << v.w << " }";
		return o;
	}
}

#endif
