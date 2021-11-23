#ifndef _MAT4_H_
# define _MAT4_H_

#include <iostream>
#include <iomanip>
#include "mft/mat.hpp"
#include "mft/vec/vec4.hpp"

namespace mft
{
	template <typename T>
	struct mat<4, 4, T>
	{
		typedef vec<4, T> col;
		col	data[4];

		mat() : data{	col(static_cast<T>(1), 0, 0, 0),
						col(0, static_cast<T>(1), 0, 0),
						col(0, 0, static_cast<T>(1), 0),
						col(0, 0, 0, static_cast<T>(1)) }
		{

		}

		mat(mat<4, 4, T> const& m)
			: data{ col(m[0]), col(m[1]), col(m[2]), col(m[3]) }
		{

		}

		mat(T scalar)
			: data{	col(scalar, 0, 0, 0),
					col(0, scalar, 0, 0),
					col(0, 0, scalar, 0),
					col(0, 0, 0, scalar)}
		{

		}

		mat(T x1, T y1, T z1, T w1,
			T x2, T y2, T z2, T w2,
			T x3, T y3, T z3, T w3,
			T x4, T y4, T z4, T w4)
			: data{	col(x1, y1, z1, w1),
					col(x2, y2, z2, w2),
					col(x3, y3, z3, w3),
					col(x4, y4, z4, w4) }
		{

		}

		col& operator[](int i)
		{
			if (!(i >= 0 && i < 4))
				return data[0];
			return data[i];
		}

		col const& operator[](int i) const
		{
			if (!(i >= 0 && i < 4))
				return data[0];
			return data[i];
		}

	};

	template <typename T>
	mat<4, 4, T> operator*(mat<4, 4, T> const& m, vec<4, T> const& v)
	{
		return vec<4, T>(
			m[0][0] * v.x + m[1][0] * v.y + m[2][0] * v.z, m[3][0] * v.w,
			m[0][1] * v.x + m[1][1] * v.y + m[2][1] * v.z, m[3][1] * v.w,
			m[0][2] * v.x + m[1][2] * v.y + m[2][2] * v.z, m[3][2] * v.w);
	}

	template <typename T>
	mat<4, 4, T> operator*(vec<4, T> const& v, mat<4, 4, T> const& m)
	{
		return vec<4, T>(
			m[0][0] * v.x + m[1][1] * v.y + m[0][2] * v.z, m[0][3] * v.w,
			m[1][0] * v.x + m[1][1] * v.y + m[1][2] * v.z, m[1][3] * v.w,
			m[2][0] * v.x + m[2][1] * v.y + m[2][2] * v.z, m[2][3] * v.w);
	}

	template <typename T>
	mat<4, 4, T> operator*(mat<4, 4, T> const& m1, mat<4, 4, T> const& m2)
	{
		mat<4, 4, T>	res;
		res[0][0] = m1[0][0] * m2[0][0] + m1[1][0] * m2[0][1] + m1[2][0] * m2[0][2] + m1[3][0] * m2[0][3];
		res[0][1] = m1[0][1] * m2[0][0] + m1[1][1] * m2[0][1] + m1[2][1] * m2[0][2] + m1[3][1] * m2[0][3];
		res[0][2] = m1[0][2] * m2[0][0] + m1[1][2] * m2[0][1] + m1[2][2] * m2[0][2] + m1[3][2] * m2[0][3];
		res[0][3] = m1[0][3] * m2[0][0] + m1[1][3] * m2[0][1] + m1[2][3] * m2[0][2] + m1[3][3] * m2[0][3];
		res[1][0] = m1[0][0] * m2[1][0] + m1[1][0] * m2[1][1] + m1[2][0] * m2[1][2] + m1[3][0] * m2[1][3];
		res[1][1] = m1[0][1] * m2[1][0] + m1[1][1] * m2[1][1] + m1[2][1] * m2[1][2] + m1[3][1] * m2[1][3];
		res[1][2] = m1[0][2] * m2[1][0] + m1[1][2] * m2[1][1] + m1[2][2] * m2[1][2] + m1[3][2] * m2[1][3];
		res[1][3] = m1[0][3] * m2[1][0] + m1[1][3] * m2[1][1] + m1[2][3] * m2[1][2] + m1[3][3] * m2[1][3];
		res[2][0] = m1[0][0] * m2[2][0] + m1[1][0] * m2[2][1] + m1[2][0] * m2[2][2] + m1[3][0] * m2[2][3];
		res[2][1] = m1[0][1] * m2[2][0] + m1[1][1] * m2[2][1] + m1[2][1] * m2[2][2] + m1[3][1] * m2[2][3];
		res[2][2] = m1[0][2] * m2[2][0] + m1[1][2] * m2[2][1] + m1[2][2] * m2[2][2] + m1[3][2] * m2[2][3];
		res[2][3] = m1[0][3] * m2[2][0] + m1[1][3] * m2[2][1] + m1[2][3] * m2[2][2] + m1[3][3] * m2[2][3];
		res[3][0] = m1[0][0] * m2[3][0] + m1[1][0] * m2[3][1] + m1[2][0] * m2[3][2] + m1[3][0] * m2[3][3];
		res[3][1] = m1[0][1] * m2[3][0] + m1[1][1] * m2[3][1] + m1[2][1] * m2[3][2] + m1[3][1] * m2[3][3];
		res[3][2] = m1[0][2] * m2[3][0] + m1[1][2] * m2[3][1] + m1[2][2] * m2[3][2] + m1[3][2] * m2[3][3];
		res[3][3] = m1[0][3] * m2[3][0] + m1[1][3] * m2[3][1] + m1[2][3] * m2[3][2] + m1[3][3] * m2[3][3];
		return res;
	}

	template <typename T>
	bool operator==(mat<4, 4, T> const& m1, mat<4, 4, T> const& m2)
	{
		return (m1[0] == m2[0] && m1[1] == m2[1] && m1[2] == m2[2] && m1[3] == m2[3]);
	}

	template <typename T>
	bool operator!=(mat<4, 4, T> const& m1, mat<4, 4, T> const& m2)
	{
		return (m1[0] != m2[0] || m1[1] != m2[1] || m1[2] != m2[2] || m1[3] != m2[3]);
	}

	template <typename T>
	std::ostream & operator<<(std::ostream& o, mat<4, 4, T> const& m)
	{
		// Retrieve the length of the biggest number to set padding

		int padding = 0;
		for (int i = 0; i < 4; i++)
		{
			int length = 0;
			int nb = static_cast<int>(m.data[i].x);
			while (nb != 0)
			{
				length++;
				nb /= 10;
			}
			if (length > padding)
				padding = length;
			length = 0;
			nb = static_cast<int>(m.data[i].y);
			while (nb != 0)
			{
				length++;
				nb /= 10;
			}
			if (length > padding)
				padding = length;
			length = 0;
			nb = static_cast<int>(m.data[i].x);
			while (nb != 0)
			{
				length++;
				nb /= 10;
			}
			if (length > padding)
				padding = length;
		}
		// Adding space for ',' and the single floating digit
		padding += 2;
		std::cout << std::fixed << std::left;
		std::cout << "{ " << std::setw(padding) << m.data[0].x;
		std::cout << "  " << std::setw(padding) << m.data[1].x;
		std::cout << "  " << std::setw(padding) << m.data[2].x;
		std::cout << "  " << std::setw(padding) << m.data[3].x;
		std::cout << " }" << std::endl;
		std::cout << "{ " << std::setw(padding) << m.data[0].y;
		std::cout << "  " << std::setw(padding) << m.data[1].y;
		std::cout << "  " << std::setw(padding) << m.data[2].y;
		std::cout << "  " << std::setw(padding) << m.data[3].y;
		std::cout << " }" << std::endl;
		std::cout << "{ " << std::setw(padding) << m.data[0].z;
		std::cout << "  " << std::setw(padding) << m.data[1].z;
		std::cout << "  " << std::setw(padding) << m.data[2].z;
		std::cout << "  " << std::setw(padding) << m.data[3].z;
		std::cout << " }" << std::endl;
		std::cout << "{ " << std::setw(padding) << m.data[0].w;
		std::cout << "  " << std::setw(padding) << m.data[1].w;
		std::cout << "  " << std::setw(padding) << m.data[2].w;
		std::cout << "  " << std::setw(padding) << m.data[3].w;
		std::cout << " }" << std::endl;
		return o;
	}
}

# endif