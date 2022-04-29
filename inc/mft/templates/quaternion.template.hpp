
#ifndef  _QUATERNIONS_TEMPLATE_HPP_
# define _QUATERNIONS_TEMPLATE_HPP_

# include "mft/quaternion.hpp"

namespace mft
{
	template<typename T>
		requires std::is_floating_point_v<T>
	constexpr quaternion<T>::quaternion( void ):
		data{1, 0, 0, 0}
	{
	}

	template<typename T>
		requires std::is_floating_point_v<T>
	constexpr quaternion<T>::quaternion( T e1, T e2, T e3, T e4 ):
		data{e1, e2, e3, e4}
	{
	}

	template<typename T>
		requires std::is_floating_point_v<T>
	constexpr quaternion<T>::quaternion( std::array<T,4> from ):
		data{from}
	{
	}

	template<typename T>
		requires std::is_floating_point_v<T>
	template<typename U>
	constexpr quaternion<T>::quaternion( const quaternion<U> from ):
		data{from.data}
	{
	}

	template<typename T>
		requires std::is_floating_point_v<T>
	constexpr quaternion<T>::quaternion( const T m[3][3] )
	{
		T t = m[0][0] + m[1][1] + m[2][2];

		if (t > static_cast<T>(0))
		{
			T s = std::sqrt(1 + t) * static_cast<T>(2.0);
			i = (m[2][1] - m[1][2]) / s;
			j = (m[0][2] - m[2][0]) / s;
			k = (m[1][0] - m[0][1]) / s;
			r = static_cast<T>(0.25) * s;
		}
		else if (m[0][0] > m[1][1] && m[0][0] > m[2][2])
		{
			T s = std::sqrt(static_cast<T>(1.0) + m[0][0] - m[1][1] - m[2][2]) * static_cast<T>(2.0);
			i = static_cast<T>(0.25) * s;
			j = (m[1][0] + m[0][1]) / s;
			k = (m[0][2] + m[2][0]) / s;
			r = (m[2][1] - m[1][2]) / s;
		}
		else if (m[1][1] > m[2][2])
		{
			T s = std::sqrt(static_cast<T>(1.0) + m[1][1] - m[0][0] - m[2][2]) * static_cast<T>(2.0);
			i = (m[1][0] + m[0][1]) / s;
			j = static_cast<T>(0.25) * s;
			k = (m[2][1] + m[1][2]) / s;
			r = (m[0][2] - m[2][0]) / s;
		}
		else
		{
			T s = std::sqrt(static_cast<T>(1.0) + m[2][2] - m[0][0] - m[1][1]) * static_cast<T>(2.0);
			i = (m[0][2] + m[2][0]) / s;
			j = (m[2][1] + m[1][2]) / s;
			k = static_cast<T>(0.25) * s;
			r = (m[1][0] - m[0][1]) / s;
		}
	}

	template<typename T>
		requires std::is_floating_point_v<T>
	constexpr quaternion<T>::quaternion( const T m[4][4] )
	{
		T t = m[0][0] + m[1][1] + m[2][2];

		if (t > static_cast<T>(0))
		{
			T s = std::sqrt(1 + t) * static_cast<T>(2.0);
			i = (m[2][1] - m[1][2]) / s;
			j = (m[0][2] - m[2][0]) / s;
			k = (m[1][0] - m[0][1]) / s;
			r = static_cast<T>(0.25) * s;
		}
		else if (m[0][0] > m[1][1] && m[0][0] > m[2][2])
		{
			T s = std::sqrt(static_cast<T>(1.0) + m[0][0] - m[1][1] - m[2][2]) * static_cast<T>(2.0);
			i = static_cast<T>(0.25) * s;
			j = (m[1][0] - m[0][1]) / s;
			k = (m[0][2] - m[2][0]) / s;
			r = (m[2][1] - m[1][2]) / s;
		}
		else if (m[1][1] > m[2][2])
		{
			T s = std::sqrt(static_cast<T>(1.0) + m[1][1] - m[0][0] - m[2][2]) * static_cast<T>(2.0);
			i = (m[1][0] - m[0][1]) / s;
			j = static_cast<T>(0.25) * s;
			k = (m[2][1] - m[1][2]) / s;
			r = (m[0][2] - m[2][0]) / s;
		}
		else
		{
			T s = std::sqrt(static_cast<T>(1.0) + m[2][2] - m[0][0] - m[1][1]) * static_cast<T>(2.0);
			i = (m[0][2] - m[2][0]) / s;
			j = (m[2][1] - m[1][2]) / s;
			k = static_cast<T>(0.25) * s;
			r = (m[1][0] - m[0][1]) / s;
		}
	}

	template<typename T>
		requires std::is_floating_point_v<T>
	quaternion<T> & quaternion<T>::operator=( const quaternion<T> & from )
	{
		std::copy(std::begin(from.data), std::end(from.data), std::begin(data));
		return *this;
	}

	template<typename T>
		requires std::is_floating_point_v<T>
	constexpr T & quaternion<T>::operator[]( size_t index )
	{
		if (index >= 0 && index < 4)
			return (data[index]);
		else
			throw std::runtime_error("Vector index out of bounds");
	}

	template<typename T>
		requires std::is_floating_point_v<T>
	constexpr const T & quaternion<T>::operator[]( size_t index ) const
	{
		if (index >= 0 && index < 4)
			return (data[index]);
		else
			throw std::runtime_error("Vector index out of bounds");
	}

	template<typename T>
		requires std::is_floating_point_v<T>
	constexpr quaternion<T> quaternion<T>::operator+( const quaternion<T> & q2 ) const
	{
		quaternion<T> ret;
		for (int i = 0; i < 4; i++)
			ret[i] = (*this)[i] + q2[i];
		return ret;
	}

	template<typename T>
		requires std::is_floating_point_v<T>
	constexpr quaternion<T> quaternion<T>::operator-( const quaternion<T> & q2 ) const
	{
		quaternion<T> ret;
		for (int i = 0; i < 4; i++)
			ret[i] = (*this)[i] - q2[i];
		return ret;
	}

	template<typename T>
		requires std::is_floating_point_v<T>
	constexpr quaternion<T> quaternion<T>::operator*( const quaternion<T> & q2 ) const
	{
		return quaternion<T>(
				a * q2.a - b * q2.b - c * q2.c - d * q2.d,
				a * q2.b + b * q2.a + c * q2.d - d * q2.c,
				a * q2.c - b * q2.d + c * q2.a + d * q2.b,
				a * q2.d + b * q2.c - c * q2.b + d * q2.a
				);
	}

	template<typename T>
	requires std::is_floating_point_v<T>
		constexpr quaternion<T> quaternion<T>::operator*(const T& scalar) const
	{
		return quaternion<T>(
			a * scalar,
			b * scalar,
			c * scalar,
			d * scalar
			);
	}

	template<typename T>
	requires std::is_floating_point_v<T>
		constexpr quaternion<T> quaternion<T>::operator/(const T& scalar) const
	{
		return quaternion<T>(
			a / scalar,
			b / scalar,
			c / scalar,
			d / scalar
			);
	}

	template<typename T>
	requires std::is_floating_point_v<T>
		constexpr quaternion<T> quaternion<T>::operator-( void ) const
	{
		return quaternion<T>(-a, -b, -c, -d);
	}

	template<typename T>
		requires std::is_floating_point_v<T>
	constexpr bool quaternion<T>::operator==( const quaternion<T> & q2 ) const
	{
		return (a == q2.a & b == q2.b & c == q2.c & d == q2.d);
	}

	template<typename T>
		requires std::is_floating_point_v<T>
	constexpr bool quaternion<T>::operator!=( const quaternion<T> & q2 ) const
	{
		return !((*this) == q2);
	}

	template<typename T>
		requires std::is_floating_point_v<T>
	std::ostream & operator<<( std::ostream & o, const quaternion<T> & q )
	{
		o << "(" << q.r << " + " << q.i << "i + " << q.j << "j + " << q.k << "k)";
		return o;
	}

	template<typename T>
		requires std::is_floating_point_v<T>
	quaternion<T> & quaternion<T>::operator+=( const quaternion<T> & q2 )
	{
		for (int i = 0; i < 4; i++)
			data[i] += q2[i];
		return *this;
	}

	template<typename T>
		requires std::is_floating_point_v<T>
	quaternion<T> & quaternion<T>::operator-=( const quaternion<T> & q2 )
	{
		for (int i = 0; i < 4; i++)
			data[i] -= q2[i];
		return *this;
	}

	template<typename T>
		requires std::is_floating_point_v<T>
	quaternion<T> & quaternion<T>::operator*=( const quaternion<T> & q2 )
	{
		(*this) = (*this) * q2;
		return *this;
	}

	template<typename T>
		requires std::is_floating_point_v<T>
	constexpr quaternion<T> quaternion<T>::rotation( const vec<T,T,T> axis, T angle )
	{
			vec<T, T, T> tmp = vec<T, T, T>::normalized(axis);
		return quaternion<T>(
				cos(angle / 2),
				tmp.x * sin(angle / 2),
				tmp.y * sin(angle / 2),
				tmp.z * sin(angle / 2)
				);
	}

	template<typename T>
	requires std::is_floating_point_v<T>
		constexpr quaternion<T> quaternion<T>::rotate( const vec<T,T,T>& rot, AngleType angleType )
	{
		quaternion<T> q;
		vec<T, T, T> finalRot = rot;
		if (angleType == AngleDegrees)
			finalRot = vec<T, T, T>(mft::radians(rot.x), mft::radians(rot.y), mft::radians(rot.z));
		if (finalRot.x != 0)
			q *= rotation(vec<T, T, T>(1.0f, 0.0f, 0.0f), finalRot.x);
		if (finalRot.y != 0)
			q *= rotation(vec<T, T, T>(0.0f, 1.0f, 0.0f), finalRot.y);
		if (finalRot.z != 0)
			q *= rotation(vec<T, T, T>(0.0f, 0.0f, 1.0f), finalRot.z);
		return q;
	}

	template<typename T>
		requires std::is_floating_point_v<T>
	constexpr vec<T, T, T> quaternion<T>::euler( const quaternion<T>& q )
	{
		vec<T, T, T> res;

		double sqw = q.w * q.w;
    double sqx = q.x * q.x;
    double sqy = q.y * q.y;
    double sqz = q.z * q.z;
		double unit = sqx + sqy + sqz + sqw;
		double test = q.x * q.y + q.z * q.w;
		if (test > 0.499 * unit)
		{ // singularity at north pole
			res.x = 2 * std::atan2(q.x, q.w);
			res.y = M_PI / 2;
			res.z = 0;
			return res;
		}
		if (test < -0.499 * unit)
		{ // singularity at south pole
			res.x = -2 * std::atan2(q.x, q.w);
			res.y = -M_PI / 2;
			res.z = 0;
			return res;
		}
	  res.y = std::atan2(2 * q.y * q.w-2 * q.x * q.z , sqx - sqy - sqz + sqw);
		res.z = std::asin(2 * test / unit);
		res.x = std::atan2(2 * q.x * q.w-2 * q.y * q.z , -sqx + sqy - sqz + sqw);
		return res;
	}

	template<typename T>
		requires std::is_floating_point_v<T>
	constexpr T quaternion<T>::dot( const quaternion<T>& x, const quaternion<T>& y )
	{
		return (x.a * y.a + x.b * y.b + x.c * y.c + x.d * y.d);
	}

	template<typename T>
		requires std::is_floating_point_v<T>
	constexpr T quaternion<T>::length( const quaternion<T>& quat)
	{
		return (sqrt(	quat.a * quat.a + quat.b * quat.b
								+ quat.c * quat.c + quat.d * quat.d));
	}

	template<typename T>
		requires std::is_floating_point_v<T>
	constexpr quaternion<T> quaternion<T>::normalized( const quaternion<T>& quat )
	{
		T	len = length(quat);
		if (len <= 0)
			return quaternion<T>();
		T inv = 1 / len;
		return quaternion<T>(inv * quat.a, inv * quat.b, inv * quat.c, inv * quat.d);
	}

	template<typename T>
		requires std::is_floating_point_v<T>
	constexpr quaternion<T> quaternion<T>::slerp( const quaternion<T>& q1,
		const quaternion<T>& q2, T percentage )
	{
		T cosTheta = dot(q1, q2);
		quaternion<T> q3 = q2;
		float epsilon = std::numeric_limits<float>::epsilon();

		if (cosTheta < 0.0)
		{
			q3 = -q2;
			cosTheta = -cosTheta;
		}
		if (cosTheta > 1.0 - epsilon)
		{
			return quaternion<T>(
				q1.a + (q3.a - q1.a) * percentage,
				q1.b + (q3.b - q1.b) * percentage,
				q1.c + (q3.c - q1.c) * percentage,
				q1.d + (q3.d - q1.d) * percentage
			);
		}
		else
		{
			float theta = std::acos(cosTheta);
			//	Why did I do this?
			if (std::sin(theta) > 0.0 - epsilon && std::sin(theta) < 0.0 + epsilon)
				std::cout << "DIVISION BY 0" << std::endl;
			return quaternion<T>(
				normalized((q1 * std::sin((1.0 - percentage) * theta)
				+ q3 * std::sin(percentage * theta) / std::sin(theta)))
			);
		}
	}
}

#endif
