#ifndef _VEC_TEMPLATE_H_
# define _VEC_TEMPLATE_H_

# include "mft/vec.hpp"
# include "mft/math_utils.hpp"
# include <cmath>
# include <stdexcept>

namespace	mft
{
	template<typename T1, typename ... Tn>
	constexpr vec<T1,Tn...>::vec( void ): vec_union<T1,Tn...>{T1(), Tn()...}
	{
	}

	template<typename T1, typename ... Tn>
	template<typename U1, typename ... Un>
	constexpr vec<T1,Tn...>::vec( const vec<U1,Un...> & from ): vec_union<T1,Tn...>{from.data}
	{
	}

	template<typename T1, typename ... Tn>
	vec<T1,Tn...> & vec<T1,Tn...>::operator=( const vec<T1,Tn...> & from )
	{
		std::copy(std::begin(from.data), std::end(from.data), std::begin(vec_union<T1,Tn...>::data));
		return *this;
	}

	template<typename T1, typename ... Tn>
	constexpr vec<T1,Tn...>::vec( const T1 scalar ): vec_union<T1,Tn...>{scalar, Tn(scalar)...}
	{
	}

	template<typename T1, typename ... Tn>
	constexpr vec<T1,Tn...>::vec( T1 e1, Tn ... en ): vec_union<T1,Tn...>{e1, en...}
	{
	}

	template<typename T1, typename ... Tn>
	constexpr T1 & vec<T1,Tn...>::operator[]( size_t index )
	{
		if (index >= 0 && index < 1 + sizeof...(Tn))
			return (vec_union<T1,Tn...>::data[index]);
		else
			throw std::runtime_error("Vector index out of bounds");
	}

	template<typename T1, typename ... Tn>
	constexpr const T1 & vec<T1,Tn...>::operator[]( size_t index ) const
	{
		if (index >= 0 && index < 1 + sizeof...(Tn))
			return (vec_union<T1,Tn...>::data[index]);
		else
			throw std::runtime_error("Vector index out of bounds");
	}

	template<typename T1, typename ... Tn>
	constexpr vec<T1,Tn...> vec<T1,Tn...>::operator+( const vec<T1,Tn...> & v2 ) const
	{
		vec<T1,Tn...> ret;
		for (int i = 0; i < 1 + sizeof...(Tn); i++)
			ret[i] = (*this)[i] + v2[i];
		return ret;
	}

	template<typename T1, typename ... Tn>
	constexpr vec<T1,Tn...> operator+( T1 scalar, const vec<T1,Tn...> & v2 )
	{
		vec<T1,Tn...> ret;
		for (int i = 0; i < 1 + sizeof...(Tn); i++)
			ret[i] = scalar + v2[i];
		return ret;
	}

	template<typename T1, typename ... Tn>
	constexpr vec<T1,Tn...> vec<T1,Tn...>::operator-( const vec<T1,Tn...> & v2 ) const
	{
		vec<T1,Tn...> ret;
		for (int i = 0; i < 1 + sizeof...(Tn); i++)
			ret[i] = (*this)[i] - v2[i];
		return ret;
	}

	template<typename T1, typename ... Tn>
	constexpr vec<T1,Tn...> operator-( T1 scalar, const vec<T1,Tn...> & v2 )
	{
		vec<T1,Tn...> ret;
		for (int i = 0; i < 1 + sizeof...(Tn); i++)
			ret[i] = scalar - v2[i];
		return ret;
	}

	template<typename T1, typename ... Tn>
	constexpr vec<T1,Tn...> vec<T1,Tn...>::operator*( const vec<T1,Tn...> & v2 ) const
	{
		vec<T1,Tn...> ret;
		for (int i = 0; i < 1 + sizeof...(Tn); i++)
			ret[i] = (*this)[i] * v2[i];
		return ret;
	}

	template<typename T1, typename ... Tn>
	constexpr vec<T1,Tn...> operator*( T1 scalar, const vec<T1,Tn...> & v2 )
	{
		vec<T1,Tn...> ret;
		for (int i = 0; i < 1 + sizeof...(Tn); i++)
			ret[i] = scalar * v2[i];
		return ret;
	}

	template<typename T1, typename ... Tn>
	constexpr vec<T1,Tn...> vec<T1,Tn...>::operator/( const vec<T1,Tn...> & v2 ) const
	{
		vec<T1,Tn...> ret;
		for (int i = 0; i < 1 + sizeof...(Tn); i++)
			ret[i] = (*this)[i] / v2[i];
		return ret;
	}

	template<typename T1, typename ... Tn>
	constexpr vec<T1,Tn...> operator/( T1 scalar, const vec<T1,Tn...> & v2 )
	{
		vec<T1,Tn...> ret;
		for (int i = 0; i < 1 + sizeof...(Tn); i++)
			ret[i] = scalar / v2[i];
		return ret;
	}

	template<typename T1, typename ... Tn>
	constexpr bool vec<T1,Tn...>::operator==( const vec<T1,Tn...> & v2 ) const
	{
		bool ret = true;
		for (int i = 0; i < 1 + sizeof...(Tn); i++)
			ret &= (*this)[i] == v2[i];
		return ret;
	}

	template<typename T1, typename ... Tn>
	constexpr bool operator==( T1 scalar, const vec<T1,Tn...> & v2 )
	{
		bool ret = true;
		for (int i = 0; i < 1 + sizeof...(Tn); i++)
			ret &= scalar == v2[i];
		return ret;
	}

	template<typename T1, typename ... Tn>
	constexpr bool vec<T1,Tn...>::operator!=( const vec<T1,Tn...> & v2 ) const
	{
		return !((*this) == v2);
	}

	template<typename T1, typename ... Tn>
	constexpr bool operator!=( T1 scalar, const vec<T1,Tn...> & v2 )
	{
		return !(scalar == v2);
	}

	template<typename T1, typename ... Tn>
	constexpr vec<T1,Tn...> operator!=( const vec<T1,Tn...> & v1, const vec<T1,Tn...> & v2 )
	{
		return !(v1 == v2);
	}

	template<typename T1, typename ... Tn>
	std::ostream & operator<<( std::ostream & o, const vec<T1,Tn...> & v )
	{
		o << "{ ";
		for (int i = 0; i < 1 + sizeof...(Tn); i++)
			o << (i > 0 ? ", " : "") << v[i];
		o << " }";
		return o;
	}

	template<typename T1, typename ... Tn>
	vec<T1,Tn...>::operator T1*()
	{
		return vec_union<T1,Tn...>::data.data();
	}

	template<typename T1, typename ... Tn>
	vec<T1,Tn...> & vec<T1,Tn...>::operator+=( const vec<T1,Tn...> & v2 )
	{
		for (int i = 0; i < 1 + sizeof...(Tn); i++)
			vec_union<T1,Tn...>::data[i] += v2[i];
		return *this;
	}

	template<typename T1, typename ... Tn>
	vec<T1,Tn...> & vec<T1,Tn...>::operator-=( const vec<T1,Tn...> & v2 )
	{
		for (int i = 0; i < 1 + sizeof...(Tn); i++)
			vec_union<T1,Tn...>::data[i] -= v2[i];
		return *this;
	}

	template<typename T1, typename ... Tn>
	vec<T1,Tn...> & vec<T1,Tn...>::operator*=( const vec<T1,Tn...> & v2 )
	{
		for (int i = 0; i < 1 + sizeof...(Tn); i++)
			vec_union<T1,Tn...>::data[i] *= v2[i];
		return *this;
	}

	template<typename T1, typename ... Tn>
	vec<T1,Tn...> & vec<T1,Tn...>::operator/=( const vec<T1,Tn...> & v2 )
	{
		for (int i = 0; i < 1 + sizeof...(Tn); i++)
			vec_union<T1,Tn...>::data[i] /= v2[i];
		return *this;
	}

	template <typename T1, typename ... Tn>
	constexpr T1 vec<T1,Tn...>::length( const vec<T1,Tn...> & v )
	{
		return std::sqrt(vec<T1,Tn...>::dot(v, v));
	}

	//	Normalized vector = v / |v|
	//	wich is also v * 1 / |v|
	//	and |v| is sqrt(v.x * v.x + ... + v.n * v.n)
	//	which is sqrt(dot(v, v))
	//	that gives us v * 1 / sqrt(v * v)
	//	ie v * rsqrt(v * v)
	template <typename T1, typename ... Tn>
	constexpr vec<T1,Tn...> vec<T1,Tn...>::normalized( const vec<T1,Tn...> & v )
	{
		return (v * rsqrt(vec<T1,Tn...>::dot(v, v)));
	}

	template<typename T1, typename ... Tn>
	constexpr T1 vec<T1,Tn...>::dot( const vec<T1,Tn...> & v1, const vec<T1,Tn...> & v2 )
	{
		T1 ret = 0; // Default-intialized
		for (int i = 0; i < 1 + sizeof...(Tn); i++)
			ret += v1[i] * v2[i];
		return ret;
	}

	template<typename T1, typename ... Tn>
	constexpr vec<T1,T1,T1> vec<T1,Tn...>::cross( const vec<T1,T1,T1> & v1, const vec<T1,T1,T1> & v2 )
	{
		return vec<T1,T1,T1>(
			(v1.y * v2.z) - (v1.z * v2.y),
			(v1.z * v2.x) - (v1.x * v2.z),
			(v1.x * v2.y) - (v1.y * v2.x));
	}
}

#endif
