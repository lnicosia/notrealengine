#ifndef _VEC2_H_
# define _VEC2_H_

#include <iostream>
#include "mft/vec.hpp"

namespace	mft
{
	template <typename T>
	struct	vec<2, T>
	{
		T	x;
		T	y;

		vec() : x(0), y(0)
		{

		}

		vec(vec<2, T> const& v) : x(v.x), y(v.y)
		{

		}

		vec(T x, T y, T z) : x(x), y(y)
		{

		}

		vec(T scalar) : x(scalar), y(scalar)
		{

		}

		template <typename X, typename Y>
		vec(X x, Y y) :
			x(static_cast<T>(x)),
			y(static_cast<T>(y))
		{

		}

		T& operator[](int i)
		{
			if (!(i >= 0 && i < 2))
				return x;
			switch (i)
			{
			default:
			case 0:
				return x;
			case 1:
				return y;
			}
		}

		T const& operator[](int i) const
		{
			if (!(i >= 0 && i < 2))
				return x;
			switch (i)
			{
			default:
			case 0:
				return x;
			case 1:
				return y;
			}
		}

		template <typename U>
		vec<2, T>& operator=(vec<2, U> const& v)
		{
			x = static_cast<T>(v.x);
			y = static_cast<T>(v.y);
			return *this;
		}

		template <typename U>
		vec<2, T>& operator*=(U scalar)
		{
			x *= static_cast<T>(scalar);
			y *= static_cast<T>(scalar);
			return *this;
		}

		template <typename U>
		vec<2, T>& operator/=(U scalar)
		{
			x /= static_cast<T>(scalar);
			y /= static_cast<T>(scalar);
			return *this;
		}

		static int size()
		{
			return 2;
		}
	};

	// Binary operators

	template <typename T>
	vec<2, T> operator+(vec<2, T> const& v, T scalar)
	{
		return vec<2, T>(
			v.x + scalar,
			v.y + scalar);
	}

	template <typename T>
	vec<2, T> operator+(T scalar, vec<2, T> const& v)
	{
		return vec<2, T>(
			scalar + v.x,
			scalar + v.y);
	}

	template <typename T>
	vec<2, T> operator+(vec<2, T> const& v1, vec<2, T> const& v2)
	{
		return vec<2, T>(
			v1.x + v2.x,
			v1.y + v2.y);
	}

	template <typename T>
	vec<2, T> operator-(vec<2, T> const& v, T scalar)
	{
		return vec<2, T>(
			v.x - scalar,
			v.y - scalar);
	}

	template <typename T>
	vec<2, T> operator-(T scalar, vec<2, T> const& v)
	{
		return vec<2, T>(
			scalar - v.x,
			scalar - v.y);
	}

	template <typename T>
	vec<2, T> operator-(vec<2, T> const& v1, vec<2, T> const& v2)
	{
		return vec<2, T>(
			v1.x - v2.x,
			v1.y - v2.y);
	}

	template <typename T>
	vec<2, T> operator*(vec<2, T> const& v, T scalar)
	{
		return vec<2, T>(
			v.x * scalar,
			v.y * scalar);
	}

	template <typename T>
	vec<2, T> operator*(T scalar, vec<2, T> const& v)
	{
		return vec<2, T>(
			scalar * v.x,
			scalar * v.y);
	}

	template <typename T>
	vec<2, T> operator*(vec<2, T> const& v1, vec<2, T> const& v2)
	{
		return vec<2, T>(
			v1.x * v2.x,
			v1.y * v2.y);
	}

	template <typename T>
	vec<2, T> operator/(vec<2, T> const& v, T scalar)
	{
		return vec<2, T>(
			v.x / scalar,
			v.y / scalar);
	}

	template <typename T>
	vec<2, T> operator/(T scalar, vec<2, T> const& v)
	{
		return vec<2, T>(
			scalar / v.x,
			scalar / v.y);
	}

	template <typename T>
	vec<2, T> operator/(vec<2, T> const& v1, vec<2, T> const& v2)
	{
		return vec<2, T>(
			v1.x / v2.x,
			v1.y / v2.y);
	}

	//	Bit operators, needed for fast rsqrt calculations

	template <typename T>
	vec<2, T> operator<<(vec<2, T> const& v1, T scalar)
	{
		return vec<2, T>(
			v1.x << scalar,
			v1.y << scalar);
	}

	template <typename T>
	vec<2, T> operator<<(T scalar, vec<2, T> const& v1)
	{
		return vec<2, T>(
			scalar << v1.x,
			scalar << v1.y);
	}

	template <typename T>
	vec<2, T> operator>>(vec<2, T> const& v1, T scalar)
	{
		return vec<2, T>(
			v1.x >> scalar,
			v1.y >> scalar);
	}

	template <typename T>
	vec<2, T> operator>>(T scalar, vec<2, T> const& v1)
	{
		return vec<2, T>(
			scalar >> v1.x,
			scalar >> v1.y);
	}

	template <typename T>
	bool operator==(vec<2, T> const& v1, vec<2, T> const& v2)
	{
		return (v1.x == v2.x && v1.y == v2.y);
	}

	template <typename T>
	bool operator!=(vec<2, T> const& v1, vec<2, T> const& v2)
	{
		return (v1.x != v2.x || v1.y != v2.y);
	}

	template <typename T>
	std::ostream& operator<<(std::ostream& o, vec<2, T> const& v)
	{
		std::cout << "{ " << v.x << " " << v.y << " }";
		return o;
	}
}

#endif
