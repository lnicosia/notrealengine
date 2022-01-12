
#ifndef  _QUATERNIONS_HPP_
# define _QUATERNIONS_HPP_

# include <type_traits>
# include "mft/templates/vec.template.hpp"

namespace mft
{
	template<typename T>
		requires std::is_floating_point_v<T>
	struct quaternion; // Pre-declaration to declare friend operators

	template<typename T>
	std::ostream & operator<<( std::ostream & o, const quaternion<T> & v );

	template<typename T>
		requires std::is_floating_point_v<T>
	struct quaternion
	{
		union {
			std::array<T,4> data;
			struct { T r, i, j, k; };
			struct { T a, b, c, d; };
		};

		// Default and copy constructors and assignement operator
		constexpr quaternion<T>( void );
		constexpr quaternion<T>( T e1, T e2, T e3, T e4 );
		constexpr quaternion<T>( std::array<T,4> from );
		template<typename U>
		constexpr quaternion<T>( const quaternion<U> from );
		quaternion<T> & operator=( const quaternion<T> & from );

		// Accessor
		constexpr T & operator[]( size_t index );
		constexpr const T & operator[]( size_t index ) const;

		// Arithemetic operators
		constexpr quaternion<T>		operator+( const quaternion<T> & v2 ) const;
		constexpr quaternion<T>		operator-( const quaternion<T> & v2 ) const;
		constexpr quaternion<T>		operator*( const quaternion<T> & v2 ) const;

		constexpr bool				operator==( const quaternion<T> & v2 ) const;
		constexpr bool				operator!=( const quaternion<T> & v2 ) const;

		// Other operators
		friend std::ostream & operator<< <>( std::ostream & o, const quaternion<T> & v );

		// Assignement operators
		quaternion<T> & operator+=( const quaternion<T> & v2 );
		quaternion<T> & operator-=( const quaternion<T> & v2 );
		quaternion<T> & operator*=( const quaternion<T> & v2 );

		static constexpr quaternion<T> rotate( const vec<T,T,T> axis, T radians);
	};
}

#endif
