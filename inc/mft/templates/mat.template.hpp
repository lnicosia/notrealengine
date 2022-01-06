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

	template<typename T1, typename ... Tn>
	mat<T1,Tn...>::operator const T1*() const
	{
		return static_cast<const T1 *>(&(*this)[0][0]);
	}

	template <typename T1, typename ... Tn>
	constexpr vec<T1,Tn...> mat<T1,Tn...>::operator*( const vec<T1,Tn...> & v ) const
	{
		vec<T1,Tn...> ret;

		for (int i = 0; i <= sizeof...(Tn); i++)
			for (int j = 0; j <= sizeof...(Tn); j++)
				ret[i][j] = 0;

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
		mat<T1,Tn...> ret;

		for (int i = 0; i <= sizeof...(Tn); i++)
			for (int j = 0; j <= sizeof...(Tn); j++)
				ret[i][j] = 0;

		for (int i = 0; i <= sizeof...(Tn); i++)
			for (int j = 0; j <= sizeof...(Tn); j++)
				for (int k = 0; k <= sizeof...(Tn); k++)
				{
					ret[i][j] += (*this)[k][j] * m2[i][k];
				}
		return ret;
	}

	template <typename T1, typename ... Tn>
	constexpr mat<T1, Tn...> mat<T1, Tn...>::operator*(const T1 & scalar) const
	{
		mat<T1, Tn...> ret;

		for (int i = 0; i <= sizeof...(Tn); i++)
			for (int j = 0; j <= sizeof...(Tn); j++)
				ret[i][j] = (*this)[i][j] * scalar;

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
	constexpr mat<T1,T1,T1,T1> mat<T1,Tn...>::lookAt( const vec<T1,T1,T1> & pos, const vec<T1,T1,T1> & target, const vec<T1,T1,T1> & up)
	{ 
		using vec3 = vec<T1, T1, T1>;
		vec3 const Forward = vec3::normalized(target - pos);
		vec3 const Right = vec3::normalized(vec3::cross(Forward, up));
		vec3 const Up = vec3::cross(Right, Forward);

		return mat<T1, T1, T1, T1>(
			{ Right.x,		Right.y,	Right.z,	-vec3::dot(Right, pos)},
			{ Up.x,			Up.y,		Up.z,		-vec3::dot(Up, pos)},
			{ -Forward.x,	-Forward.y,	-Forward.z,	vec3::dot(Forward, pos)},
			{ 0,			0,			0,			1});
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
				{ 0					, 0					, 1 / (far - near)	  ,	-near / (far - near)			 },
				{ 0					, 0					, 0					  , 1								 }
				);
	 }

	template<typename T1, typename ... Tn>
	constexpr mat<T1,T1,T1,T1> mat<T1,Tn...>::perspective( const T1 fovy, const T1 aspect, const T1 near, const T1 far)
	{ 
		const T1 tang = std::tan(fovy * 0.5f);

		return mat<T1, T1, T1, T1>(
			{ 1 / (aspect * tang), 0	   , 0	  , 0 },
			{ 0					 , 1 / tang, 0	  , 0 },
			{ 0					 , 0	   , far / (far - near)	  , -(far * near) / (far - near) },
			{ 0					 , 0	   , 1, 0 }
		);
	}

	// Does not work??
	template<typename T1, typename ... Tn>
	constexpr mat<T1, T1, T1, T1> mat<T1, Tn...>::perspective(const T1 left, const T1 right, const T1 bottom, const T1 top, const T1 near, const T1 far)
	{
		return mat<T1, T1, T1, T1>(
			{ (2 * near) / (right - left),	0,	(right + left) / (right - left), 0 },
			{ 0,			(2 * near) / (top - bottom), (top + bottom) / (top - bottom), 0 },
			{ 0,			0,			far / (far - near),	-(far * near) / (far - near) },
			{ 0,			0,			1,								0 }
		);
	}

	template<typename T1, typename ... Tn>
	constexpr mat<T1, T1, T1, T1> mat<T1, Tn...>::transpose(const mat<T1, T1, T1, T1>& m)
	{
		return mat<T1, T1, T1, T1>(
			{ m[0][0], m[1][0], m[2][0], m[3][0] },
			{ m[0][1], m[1][1], m[2][1], m[3][1] },
			{ m[0][2], m[1][2], m[2][2], m[3][2] },
			{ m[0][3], m[1][3], m[2][3], m[3][3] }
		);
	}

	template<typename T1, typename ... Tn>
	constexpr mat<T1, T1, T1, T1> mat<T1, Tn...>::inverse(const mat<T1, T1, T1, T1>& m2)
	{
		mat<T1, T1, T1, T1> m = transpose(m2);
		T1 Coef00 = m[2][2] * m[3][3] - m[3][2] * m[2][3];
		T1 Coef02 = m[1][2] * m[3][3] - m[3][2] * m[1][3];
		T1 Coef03 = m[1][2] * m[2][3] - m[2][2] * m[1][3];

		T1 Coef04 = m[2][1] * m[3][3] - m[3][1] * m[2][3];
		T1 Coef06 = m[1][1] * m[3][3] - m[3][1] * m[1][3];
		T1 Coef07 = m[1][1] * m[2][3] - m[2][1] * m[1][3];
		
		T1 Coef08 = m[2][1] * m[3][2] - m[3][1] * m[2][2];
		T1 Coef10 = m[1][1] * m[3][2] - m[3][1] * m[1][2];
		T1 Coef11 = m[1][1] * m[2][2] - m[2][1] * m[1][2];

		T1 Coef12 = m[2][0] * m[3][3] - m[3][0] * m[2][3];
		T1 Coef14 = m[1][0] * m[3][3] - m[3][0] * m[1][3];
		T1 Coef15 = m[1][0] * m[2][3] - m[2][0] * m[1][3];

		T1 Coef16 = m[2][0] * m[3][2] - m[3][0] * m[2][2];
		T1 Coef18 = m[1][0] * m[3][2] - m[3][0] * m[1][2];
		T1 Coef19 = m[1][0] * m[2][2] - m[2][0] * m[1][2];

		T1 Coef20 = m[2][0] * m[3][1] - m[3][0] * m[2][1];
		T1 Coef22 = m[1][0] * m[3][1] - m[3][0] * m[1][1];
		T1 Coef23 = m[1][0] * m[2][1] - m[2][0] * m[1][1];

		vec<T1,T1,T1,T1> Fac0(Coef00, Coef00, Coef02, Coef03);
		vec<T1,T1,T1,T1> Fac1(Coef04, Coef04, Coef06, Coef07);
		vec<T1,T1,T1,T1> Fac2(Coef08, Coef08, Coef10, Coef11);
		vec<T1,T1,T1,T1> Fac3(Coef12, Coef12, Coef14, Coef15);
		vec<T1,T1,T1,T1> Fac4(Coef16, Coef16, Coef18, Coef19);
		vec<T1,T1,T1,T1> Fac5(Coef20, Coef20, Coef22, Coef23);

		vec<T1,T1,T1,T1> Vec0(m[1][0], m[0][0], m[0][0], m[0][0]);
		vec<T1,T1,T1,T1> Vec1(m[1][1], m[0][1], m[0][1], m[0][1]);
		vec<T1,T1,T1,T1> Vec2(m[1][2], m[0][2], m[0][2], m[0][2]);
		vec<T1,T1,T1,T1> Vec3(m[1][3], m[0][3], m[0][3], m[0][3]);

		vec<T1,T1,T1,T1> Inv0(Vec1 * Fac0 - Vec2 * Fac1 + Vec3 * Fac2);
		vec<T1,T1,T1,T1> Inv1(Vec0 * Fac0 - Vec2 * Fac3 + Vec3 * Fac4);
		vec<T1,T1,T1,T1> Inv2(Vec0 * Fac1 - Vec1 * Fac3 + Vec3 * Fac5);
		vec<T1,T1,T1,T1> Inv3(Vec0 * Fac2 - Vec1 * Fac4 + Vec2 * Fac5);

		vec<T1,T1,T1,T1> SignA(+1, -1, +1, -1);
		vec<T1,T1,T1,T1> SignB(-1, +1, -1, +1);
		mat<T1,T1,T1,T1> Inverse(Inv0 * SignA, Inv1 * SignB, Inv2 * SignA, Inv3 * SignB);

		vec<T1,T1,T1,T1> Row0(Inverse[0][0], Inverse[1][0], Inverse[2][0], Inverse[3][0]);

		vec<T1,T1,T1,T1> Dot0(m[0] * Row0);
		T1 Dot1 = (Dot0.x + Dot0.y) + (Dot0.z + Dot0.w);

		T1 OneOverDeterminant = static_cast<T1>(1) / Dot1;

		return transpose(Inverse * OneOverDeterminant);
	}
}

# endif
