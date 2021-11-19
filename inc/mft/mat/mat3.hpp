#ifndef _MAT3_H_
# define _MAT3_H_

#include <iostream>
#include <iomanip>
#include "mft/mat.hpp"

namespace mft
{
	template <typename T>
	struct mat<3, 3, T>
	{
		typedef vec<3, T> col;
		col	data[3];

		mat() : data {	col(static_cast<T>(1), 0, 0),
						col(0, static_cast<T>(1), 0),
						col(0, 0, static_cast<T>(1)) }
		{

		}

		mat(mat<3, 3, T> const& m)
			: data { col(m[0]), col(m[1]), col(m[2]) }
		{

		}

		template <typename U>
		mat(U scalar)
			: data {col(static_cast<T>(scalar), 0, 0),
					col(0, static_cast<T>(scalar), 0),
					col(0, 0, static_cast<T>(scalar))}
		{

		}

		mat(T x1, T y1, T z1,
			T x2, T y2, T z2,
			T x3, T y3, T z3)
			: data{ col(x1, y1, z1), col(x2, y2, z2), col(x3, y3, z3) }
		{

		}

		col & operator[](int i)
		{
			if (!(i >= 0 && i < 3))
				return data[0];
			return data[i];
		}

		col const& operator[](int i) const
		{
			if (!(i >= 0 && i < 3))
				return data[0];
			return data[i];
		}

	};

	template <typename T>
	mat<3, 3, T> operator*(mat<3, 3, T> const& m, vec<3, T> const& v)
	{
		return vec<3, T>(
			m[0][0] * v.x + m[1][0] * v.y + m[2][0] * v.z,
			m[0][1] * v.x + m[1][1] * v.y + m[2][1] * v.z,
			m[0][2] * v.x + m[1][2] * v.y + m[2][2] * v.z);
	}

	template <typename T>
	mat<3, 3, T> operator*(vec<3, T> const& v, mat<3, 3, T> const& m)
	{
		return vec<3, T>(
			m[0][0] * v.x + m[1][1] * v.y + m[0][2] * v.z,
			m[1][0] * v.x + m[1][1] * v.y + m[1][2] * v.z,
			m[2][0] * v.x + m[2][1] * v.y + m[2][2] * v.z);
	}

	template <typename T>
	mat<3, 3, T> operator*(mat<3, 3, T> const& m1, mat<3, 3, T> const& m2)
	{
		mat<3, 3, T>	res;
		res[0][0] = m1[0][0] * m2[0][0] + m1[1][0] * m2[0][1] + m1[2][0] * m2[0][2];
		res[0][1] = m1[0][1] * m2[0][0] + m1[1][1] * m2[0][1] + m1[2][1] * m2[0][2];
		res[0][2] = m1[0][2] * m2[0][0] + m1[1][2] * m2[0][1] + m1[2][2] * m2[0][2];
		res[1][0] = m1[0][0] * m2[1][0] + m1[1][0] * m2[1][1] + m1[2][0] * m2[1][2];
		res[1][1] = m1[0][1] * m2[1][0] + m1[1][1] * m2[1][1] + m1[2][1] * m2[1][2];
		res[1][2] = m1[0][2] * m2[1][0] + m1[1][2] * m2[1][1] + m1[2][2] * m2[1][2];
		res[2][0] = m1[0][0] * m2[2][0] + m1[1][0] * m2[2][1] + m1[2][0] * m2[2][2];
		res[2][1] = m1[0][1] * m2[2][0] + m1[1][1] * m2[2][1] + m1[2][1] * m2[2][2];
		res[2][2] = m1[0][2] * m2[2][0] + m1[1][2] * m2[2][1] + m1[2][2] * m2[2][2];
		return res;
	}

	template <typename T>
	bool operator==(mat<3, 3, T> const& m1, mat<3, 3, T> const& m2)
	{
		return (m1[0] == m2[0] && m1[1] == m2[1] && m1[2] == m2[2]);
	}

	template <typename T>
	bool operator!=(mat<3, 3, T> const& m1, mat<3, 3, T> const& m2)
	{
		return (m1[0] != m2[0] || m1[0] != m2[0] || m1[0] != m2[0]);
	}

	template <typename T>
	std::ostream& operator<<(std::ostream& o, mat<3, 3, T> const& m)
	{
		// Retrieve the length of the biggest number to set padding

		int padding = 0;
		for (int i = 0; i < 3; i++)
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
		std::cout << std::fixed << std::setprecision(1) << std::left;
		std::cout << "{ " << std::setw(padding) << m.data[0].x;
		std::cout << "  " << std::setw(padding) << m.data[1].x;
		std::cout << "  " << std::setw(padding) << m.data[2].x;
		std::cout << " }" << std::endl;
		std::cout << "{ " << std::setw(padding) << m.data[0].y;
		std::cout << "  " << std::setw(padding) << m.data[1].y;
		std::cout << "  " << std::setw(padding) << m.data[2].y;
		std::cout << " }" << std::endl;
		std::cout << "{ " << std::setw(padding) << m.data[0].z;
		std::cout << "  " << std::setw(padding) << m.data[1].z;
		std::cout << "  " << std::setw(padding) << m.data[2].z;
		std::cout << " }" << std::endl;
		return o;
	}
}

# endif