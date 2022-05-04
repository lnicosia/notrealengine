#ifndef _VEC_H_
# define _VEC_H_

# define _USE_MATH_DEFINES
# include <cmath>
# include <cstddef>
# include <type_traits>
# include <initializer_list>
# include <ostream>
# include <array>

namespace	mft
{
	template<typename T1, typename ... Tn>
		requires (std::is_same_v<T1,Tn> && ...)
	struct vec_union {
		std::array<T1,1 + sizeof...(Tn)> data;

		constexpr vec_union<T1,Tn...>( T1 e1, Tn ... en ): data{e1, en...} {};
		constexpr vec_union<T1,Tn...>( std::array<T1,1 + sizeof...(Tn)> from ): data(from) {};
	};

	template<typename T>
	struct vec_union<T,T> {
		union {
			std::array<T,2> data;
			struct { T x, y; };
			struct { T u, v; };
		};
		constexpr vec_union<T,T>( T e1, T e2 ): data{e1, e2} {};
		constexpr vec_union<T,T>( std::array<T,2> from ): data(from) {};
	};

	template<typename T>
	struct vec_union<T,T,T> {
		union {
			std::array<T,3> data;
			struct { T x, y, z; };
			struct { T s, t, p; };
			struct { T r, g, b; };
		};
		constexpr vec_union<T,T,T>( T e1, T e2, T e3 ): data{e1, e2, e3} {};
		constexpr vec_union<T,T,T>( std::array<T,3> from ): data(from) {};
	};

	template<typename T>
	struct vec_union<T,T,T,T> {
		union {
			std::array<T,4> data;
			struct { T x, y, z, w; };
			struct { T r, g, b, a; };
		};
		constexpr vec_union<T,T,T,T>( T e1, T e2, T e3, T e4 ): data{e1, e2, e3, e4} {};
		constexpr vec_union<T,T,T,T>( std::array<T,4> from ): data(from) {};
	};


	template<typename T1, typename ... Tn>
	struct vec; // Pre-declaration to declare friend operators

	// Friend operators
	template<typename T1, typename ... Tn>
	constexpr vec<T1,Tn...> operator+( T1, const vec<T1,Tn...> & v2 );
	template<typename T1, typename ... Tn>
	constexpr vec<T1,Tn...> operator-( T1, const vec<T1,Tn...> & v2 );
	template<typename T1, typename ... Tn>
	constexpr vec<T1,Tn...> operator*( T1, const vec<T1,Tn...> & v2 );
	template<typename T1, typename ... Tn>
	constexpr vec<T1,Tn...> operator/( T1, const vec<T1,Tn...> & v2 );

	template<typename T1, typename ... Tn>
	constexpr bool operator==( T1, const vec<T1,Tn...> & v2 );
	template<typename T1, typename ... Tn>
	constexpr bool operator!=( T1, const vec<T1,Tn...> & v2 );

	template<typename T1, typename ... Tn>
	std::ostream & operator<<( std::ostream & o, const vec<T1,Tn...> & v );


	template<typename T1, typename ... Tn>
	struct vec : public vec_union<T1,Tn...> {

		// Default and copy constructors and assignement operator
		constexpr vec<T1,Tn...>( void );
		// This templated constructor adds implicit conversion from any other compatible type
		template<typename U1, typename ... Un>
		constexpr vec<T1,Tn...>( const vec<U1,Un...> & from );
		vec<T1,Tn...> & operator=( const vec<T1,Tn...> & from );

		// Scalar and list initializers
		constexpr vec<T1,Tn...>( const T1 scalar );
		constexpr vec<T1,Tn...>( T1 e1, Tn ... en );

		// Accessor
		constexpr T1 & operator[]( size_t index );
		constexpr const T1 & operator[]( size_t index ) const;

		// Conversion to data pointer
		explicit operator T1*();

		// Arithemetic operators
		// No need to define vec/scalar operations as scalars can
		// implicitly be converted to vectors through the constructor
		constexpr vec<T1,Tn...>			operator+( const vec<T1,Tn...> & v2 ) const;
		constexpr vec<T1,Tn...>			operator-( const vec<T1,Tn...> & v2 ) const;
		constexpr vec<T1,Tn...>			operator*( const vec<T1,Tn...> & v2 ) const;
		constexpr vec<T1,Tn...>			operator/( const vec<T1,Tn...> & v2 ) const;

		constexpr bool					operator==( const vec<T1,Tn...> & v2 ) const;
		constexpr bool					operator!=( const vec<T1,Tn...> & v2 ) const;

		// Other operators
		friend std::ostream & operator<< <>( std::ostream & o, const vec<T1,Tn...> & v );

		// Assignement operators
		vec<T1,Tn...> & operator+=( const vec<T1,Tn...> & v2 );
		vec<T1,Tn...> & operator-=( const vec<T1,Tn...> & v2 );
		vec<T1,Tn...> & operator*=( const vec<T1,Tn...> & v2 );
		vec<T1,Tn...> & operator/=( const vec<T1,Tn...> & v2 );

		// Vector-specific operations
		static constexpr T1 length( const vec<T1,Tn...> & v );
		static constexpr vec<T1,Tn...> normalized( const vec<T1,Tn...> & v );
		static constexpr T1 dot( const vec<T1,Tn...> & v1, const vec<T1,Tn...> & v2 );
		static constexpr vec<T1,T1,T1> cross( const vec<T1,T1,T1> & v1, const vec<T1,T1,T1> & v2 );
	};
	
	template<typename T1, typename ... Tn>
	constexpr vec<T1, Tn...>	operator+(T1, const vec<T1, Tn...>& v2);
	template<typename T1, typename ... Tn>
	constexpr vec<T1, Tn...>	operator-(T1, const vec<T1, Tn...>& v2);
	template<typename T1, typename ... Tn>
	constexpr vec<T1, Tn...>	operator*(T1, const vec<T1, Tn...>& v2);
	template<typename T1, typename ... Tn>
	constexpr vec<T1, Tn...>	operator/(T1, const vec<T1, Tn...>& v2);

	template<typename T1, typename ... Tn>
	constexpr bool			operator==(T1, const vec<T1, Tn...>& v2);
	template<typename T1, typename ... Tn>
	constexpr bool			operator!=(T1, const vec<T1, Tn...>& v2);
}

#endif
