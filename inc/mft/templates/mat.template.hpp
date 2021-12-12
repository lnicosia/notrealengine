#ifndef _MAT_TEMPLATE_H_
# define _MAT_TEMPLATE_H_

#include <iostream>
#include <iomanip>
#include <cmath>
#include "mft/mat.hpp"

namespace mft
{
	template<typename T1, typename ... Tn>
	constexpr mat<T1,Tn...>::mat( void ):
		Rows()
	{
		for (int i = 0; i <= sizeof...(Tn); i++)
			Rows::data[i][i] = T1(1);
	}

	template<typename T1, typename ... Tn>
	template<typename U1, typename... Un>
	constexpr mat<T1,Tn...>::mat( const mat<U1,Un...> & m ):
		Rows(static_cast<Rows>(m))
	{
	}

	template<typename T1, typename ... Tn>
	mat<T1,Tn...> & mat<T1,Tn...>::operator=( const mat<T1,Tn...> & from )
	{
		Rows::operator=(from);
		return *this;
	}

	template<typename T1, typename ... Tn>
	constexpr mat<T1,Tn...>::mat( const Row & r1,
								  const Row & r2,
								  const Row & r3,
								  const Row & r4 ):
		Rows(r1, r2, r3, r4)
	{
	}

	template<typename T1, typename ... Tn>
	constexpr vec<T1,Tn...> & mat<T1,Tn...>::operator[]( size_t index )
	{
		if (index >= 0 && index <= sizeof...(Tn))
			return (Rows::data[index]);
		else
			throw std::runtime_error("Matrix index out of bounds");
	}

	template<typename T1, typename ... Tn>
	constexpr const vec<T1,Tn...> & mat<T1,Tn...>::operator[]( size_t index ) const
	{
		if (index >= 0 && index <= sizeof...(Tn))
			return (Rows::data[index]);
		else
			throw std::runtime_error("Matrix index out of bounds");
	}

	template<typename T1, typename ... Tn>
	mat<T1,Tn...>::operator T1*()
	{
		return static_cast<T1 *>(&(*this)[0][0]);
	}

	template <typename T1, typename ... Tn>
	constexpr vec<T1,Tn...> mat<T1,Tn...>::operator*( const vec<T1,Tn...> & v ) const
	{
		vec<T1,Tn...> ret(	vec<T1, Tn...>(0),
							vec<T1, Tn...>(0),
							vec<T1, Tn...>(0),
							vec<T1, Tn...>(0));

		for (int i = 0; i <= sizeof...(Tn); i++)
			for (int j = 0; j <= sizeof...(Tn); j++)
			{
				ret[i] += this[i][j] * v[i];
			}
		return ret;
	}

	template <typename T1, typename ... Tn>
	constexpr mat<T1,Tn...> mat<T1,Tn...>::operator*( const mat<T1,Tn...> & m2 ) const
	{
		mat<T1,Tn...> ret(	vec<T1, Tn...>(0),
							vec<T1, Tn...>(0),
							vec<T1, Tn...>(0),
							vec<T1, Tn...>(0));

		for (int i = 0; i <= sizeof...(Tn); i++)
			for (int j = 0; j <= sizeof...(Tn); j++)
				for (int k = 0; k <= sizeof...(Tn); k++)
				{
					ret[i][j] += (*this)[k][j] * m2[i][k];
				}
		return ret;
	}

	template <typename T1, typename ... Tn>
	constexpr bool mat<T1,Tn...>::operator==( const mat<T1,Tn...> & m2 ) const
	{
		return Rows::operator==(*this, m2);
	}

	template <typename T1, typename ... Tn>
	constexpr bool mat<T1,Tn...>::operator!=( const mat<T1,Tn...> & m2 ) const
	{
		return !(*this == m2);
	}

	template <typename T1, typename ... Tn>
	std::ostream & operator<<(std::ostream & o, const mat<T1,Tn...>& m)
	{
		o << std::fixed << std::left;
		o << "{" << std::endl;
		for (int i = 0; i <= sizeof...(Tn); i++)
		{
			o << "{";
			for (int j = 0; j <= sizeof...(Tn); j++)
				o << " " << std::setw(8) << m[i][j];
			o << " }" << std::endl;
		}
		o << "}" << std::endl;
		return o;
	}

	template<typename T1, typename ... Tn>
	mat<T1,Tn...> & mat<T1,Tn...>::operator*=( const mat<T1,Tn...> & m2 )
	{
		mat<T1, Tn...> ret(	vec<T1, Tn...>(0),
							vec<T1, Tn...>(0),
							vec<T1, Tn...>(0),
							vec<T1, Tn...>(0));

		for (int i = 0; i <= sizeof...(Tn); i++)
			for (int j = 0; j <= sizeof...(Tn); j++)
				for (int k = 0; k <= sizeof...(Tn); k++)
				{
					ret[i][j] += (*this)[k][j] * m2[i][k];
				}
		*this = ret;
		return *this;
	}

	template<typename T1, typename ... Tn>
	constexpr mat<T1,T1,T1,T1> mat<T1,Tn...>::translate( const vec<T1,T1,T1> & v )
	{
		return mat<T1,T1,T1,T1>(
				{ 1, 0, 0, v.x },
				{ 0, 1, 0, v.y },
				{ 0, 0, 1, v.z },
				{ 0, 0, 0, 1 }
				);
	 }

	template<typename T1, typename ... Tn>
	constexpr mat<T1,T1,T1,T1> mat<T1,Tn...>::scale( const vec<T1,T1,T1> & v )
	{ 
		return mat<T1,T1,T1,T1>(
				{ v.x, 0, 0, 0 },
				{ 0, v.y, 0, 0 },
				{ 0, 0, v.z, 0 },
				{ 0, 0, 0, 1 }
				);
	 }

	template<typename T1, typename ... Tn>
	constexpr mat<T1,T1,T1,T1> mat<T1,Tn...>::rotate( const T1 angle, const vec<T1,T1,T1> & axis )
	{ 
		const float			c = std::cos(angle);
		const float			s = std::sin(angle);
		const float			minc = 1 - c;
		const vec<T1,T1,T1>	u = vec<T1,T1,T1>::normalized(axis);

		return mat<T1,T1,T1,T1>(
				{ c + u.x * u.x * minc, u.x * u.y * minc - u.z * s, u.x * u.z * minc + u.y * s, 0 },
				{ u.y * u.x * minc + u.z * s, c + u.y * u.y * minc, u.y * u.z * minc - u.x * s, 0 },
				{ u.z * u.x * minc - u.y * s, u.z * u.y * minc + u.x * s, c + u.z * u.z * minc, 0 },
				{ 0, 0, 0, 1 }
				);
	 }

	template<typename T1, typename ... Tn>
	constexpr mat<T1,T1,T1,T1> mat<T1,Tn...>::look_at( const vec<T1,T1,T1> & pos, const vec<T1,T1,T1> & target, const vec<T1,T1,T1> & up)
	{ 
		using vec3 = vec<T1,T1,T1>;
		vec3 right = vec3::normalized(vec3::cross(vec3::normalized(up), target));

		return mat<T1,T1,T1,T1>(
				{ right.x , right.y , right.z , 0 },
				{ up.x	  , up.y	, up.z	  , 0 },
				{ target.x, target.y, target.z, 0 },
				{ 0		  , 0		, 0		  , 1 }
				) * mat<T1, T1, T1, T1>(
					{ 1, 0, 0, -pos.x },
					{ 0, 1, 0, -pos.y },
					{ 0, 0, 1, -pos.z },
					{ 0, 0, 0, 1 });
	 }

	template<typename T1, typename ... Tn>
	constexpr mat<T1,T1,T1,T1> mat<T1,Tn...>::ortho( const T1 left, const T1 right, const T1 bottom, const T1 top )
	{ 
		return mat<T1,T1,T1,T1>(
				{ 2 / (right - left), 0					, 0, -(right + left) / (right - left) },
				{ 0					, 2 / (top - bottom), 0, -(top + bottom) / (top - bottom) },
				{ 0					, 0					, 1, 0								 },
				{ 0					, 0					, 0, 1								 }
				);
	 }

	template<typename T1, typename ... Tn>
	constexpr mat<T1,T1,T1,T1> mat<T1,Tn...>::ortho( const T1 left, const T1 right, const T1 bottom, const T1 top, const T1 near, const T1 far )
	{ 
		return mat<T1,T1,T1,T1>(
				{ 2 / (right - left), 0					, 0					  , -(right + left) / (right - left) },
				{ 0					, 2 / (top - bottom), 0					  , -(top + bottom) / (top - bottom) },
				{ 0					, 0					, -2.0f / (far - near), -(far + near) / (far - near)	 },
				{ 0					, 0					, 0					  , 1								 }
				);
	 }

	template<typename T1, typename ... Tn>
	constexpr mat<T1,T1,T1,T1> mat<T1,Tn...>::perspective( const T1 fovy, const T1 aspect, const T1 near, const T1 far)
	{ 
		const T1 tang = std::tan(fovy * 0.5f);

		return mat<T1,T1,T1,T1>(
				{ 1 / (aspect * tang), 0	   , 0	  , 0  },
				{ 0					 , 1 / tang, 0	  , 0  },
				{ 0					 , 0	   , far / (far - near)	  , 1 },
				{ 0					 , 0	   , -(far * near) / (far - near), 0  }
				);
	}

	template<typename T1, typename ... Tn>
	constexpr mat<T1, T1, T1, T1> mat<T1, Tn...>::perspective(const T1 left, const T1 right, const T1 bottom, const T1 top, const T1 near, const T1 far)
	{
		return mat<T1, T1, T1, T1>(
			{ near / right, 0	   , 0	  , 0 },
			{ 0					 , near / top, 0	  , 0 },
			{ 0					 , 0	   , -(far + near) / (far - near)	  , 1 },
			{ 0					 , 0	   , -(far * near) / (far - near), 0 }
		);
	}
}

# endif
