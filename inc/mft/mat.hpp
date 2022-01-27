#ifndef _MAT_H_
# define _MAT_H_

# include "mft/templates/vec.template.hpp"
# include "mft/templates/quaternion.template.hpp"

# include <type_traits>

namespace	mft
{
	template<typename T1, typename ... Tn>
	struct mat; // Pre-declaration to declare friend operators

	template<typename T1, typename ... Tn>
	std::ostream & operator<<( std::ostream & o, const mat<T1,Tn...> & m );

	template<typename T1,typename ... Tn>
	struct mat : private vec<vec<T1,Tn...>,std::conditional_t<true,vec<T1,Tn...>,Tn>...> {
		typedef vec<T1,Tn...> Row;
		typedef vec<Row,std::conditional_t<true,Row,Tn>...> Rows;

		// Default and copy constructors and assignement operator
		constexpr mat<T1,Tn...>( void );
		// This templated constructor adds implicit conversion from any other compatible type
		template<typename U1,typename ... Un>
		constexpr mat<T1,Tn...>( const mat<U1,Un...> & from );
		mat<T1,Tn...> & operator=( const mat<T1,Tn...> & from );

		// List initializer
		constexpr mat<T1,Tn...>( const vec<T1,Tn...> & r1,
								 const vec<T1,Tn...> & r2,
								 const vec<T1,Tn...> & r3,
								 const vec<T1,Tn...> & r4 );

		constexpr mat<T1,Tn...>( const vec<T1, Tn...>& r1,
								 const vec<T1, Tn...>& r2,
								 const vec<T1, Tn...>& r3);

		// Accessor
		constexpr vec<T1,Tn...> & operator[]( size_t index );
		constexpr const vec<T1,Tn...> & operator[]( size_t index ) const;

		// Conversion to data pointer
		explicit operator T1*();
		explicit operator const T1*() const;

		constexpr mat<T1,Tn...> operator+( const mat<T1,Tn...> & m2 ) const;
		constexpr mat<T1,Tn...> operator-( const mat<T1,Tn...> & m2 ) const;
		constexpr vec<T1,Tn...> operator*( const vec<T1,Tn...> & v ) const;
		constexpr mat<T1,Tn...> operator*( const mat<T1,Tn...> & m2 ) const;
		constexpr mat<T1, Tn...> operator*(const T1 & scalar) const;

		constexpr bool operator==( const mat<T1,Tn...> & m2 ) const;
		constexpr bool operator!=( const mat<T1,Tn...> & m2 ) const;

		// Other operators
		friend std::ostream & operator<< <>( std::ostream & o, const mat<T1,Tn...> & mat );

		mat<T1,Tn...> & operator*=( const mat<T1,Tn...> & m2 );

		// 3D operations on mat4
		static constexpr mat<T1,T1,T1,T1> inverse(const mat<T1,T1,T1,T1>& m);
		static constexpr mat<T1, T1, T1, T1> transpose(const mat<T1, T1, T1, T1>& m);
		static constexpr mat<T1,T1,T1,T1> translate( const vec<T1,T1,T1> & v );
		static constexpr mat<T1,T1,T1,T1> scale( const vec<T1,T1,T1> & v );
		static constexpr mat<T1,T1,T1,T1> rotate( const T1 angle, const vec<T1,T1,T1> & axis );
		static constexpr mat<T1,T1,T1,T1> lookAt( const vec<T1,T1,T1> & pos, const vec<T1,T1,T1> & target, const vec<T1,T1,T1> & up);
		static constexpr mat<T1,T1,T1,T1> rotate( const quaternion<T1> rotation );

		static constexpr mat<T1,T1,T1,T1> ortho( const T1 left, const T1 right, const T1 bottom, const T1 top );
		static constexpr mat<T1,T1,T1,T1> ortho( const T1 left, const T1 right, const T1 bottom, const T1 top, const T1 near, const T1 far );
		static constexpr mat<T1,T1,T1,T1> perspective( const T1 fovy, const T1 aspect, const T1 near, const T1 far);
		static constexpr mat<T1, T1, T1, T1> perspective(const T1 left, const T1 right, const T1 bottom, const T1 top, const T1 near, const T1 far);
	};
}

#endif
