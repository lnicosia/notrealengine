
#ifndef  _QUATERNIONS_HPP_
# define _QUATERNIONS_HPP_

# include "mft/templates/vec.template.hpp"
# include "mft/mat.hpp"
# include <type_traits>

namespace mft
{
	enum AngleType
	{
		AngleRadians,
		AngleDegrees,
	};

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
			struct { T w, x, y, z; };
		};

		// Default and copy constructors and assignement operator
		constexpr quaternion<T>( void );
		constexpr quaternion<T>( T e1, T e2, T e3, T e4 );
		constexpr quaternion<T>( std::array<T,4> from );
		//	Construct from a rotation matrix
		//	takes float arrays instead of matrices to avoid circular dependency
		//	between matrices and quaternions
		constexpr quaternion<T>( const T m[3][3] );
		constexpr quaternion<T>( const T m[4][4] );
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

		constexpr quaternion<T>		operator*(const T& scalar) const;
		constexpr quaternion<T>		operator/(const T& scalar) const;

		constexpr quaternion<T>		operator-( void ) const;

		constexpr bool				operator==( const quaternion<T> & v2 ) const;
		constexpr bool				operator!=( const quaternion<T> & v2 ) const;

		// Other operators
		friend std::ostream & operator<< <>( std::ostream & o, const quaternion<T> & v );

		// Assignement operators
		quaternion<T> & operator+=( const quaternion<T> & v2 );
		quaternion<T> & operator-=( const quaternion<T> & v2 );
		quaternion<T> & operator*=( const quaternion<T> & v2 );

		static constexpr quaternion<T> rotation( const vec<T,T,T> axis, T radians );
		static constexpr quaternion<T> rotate( const vec<T, T, T>& rot, AngleType = AngleRadians);
		static constexpr vec<T, T, T>	euler( const quaternion<T>& q );
		static constexpr T dot( const quaternion<T>& x, const quaternion<T>& y );
		static constexpr T length( const quaternion<T>& quat );
		static constexpr quaternion<T> normalized( const quaternion<T>& quat );
		static constexpr quaternion<T> slerp( const quaternion<T>& q1,
			const quaternion<T>& q2, T percentage );
	};
}

#endif
