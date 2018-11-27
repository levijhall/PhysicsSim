#pragma once

#include "core.h"

//TODO: matrix<0> = NULL

template <unsigned r_dim, unsigned c_dim = r_dim>
struct matrix
{
	const unsigned row = r_dim;
	const unsigned col = c_dim;
	float *d;

	matrix() : d(new float[r_dim * c_dim]()) {}

	//template <typename T>
	matrix(std::initializer_list<float> l) : d(new float[r_dim * c_dim]())
	{
		//Should be a static_assert, but it won't work...
		if (l.size() > r_dim * c_dim)
		{
			delete[] d;
			throw std::out_of_range("Too many items in initializer list.");
		}

		for (auto it = l.begin(); it != l.end(); it++)
		{
			d[it - l.begin()] = *it;
		}
	}

	//Copy constructor
	matrix(const matrix<r_dim, c_dim> &other) : d(new float[r_dim * c_dim]())
	{
		for (unsigned i = 0; i < r_dim * c_dim; i++)
		{
			d[i] = other.d[i];
		}
	}

	//Move constructor
	matrix(matrix &&other) : d(other.d)
	{
		other.d = nullptr;
	}

	matrix(const float constant) : d(new float[r_dim * c_dim]())
	{
		static_assert(r_dim == c_dim, "Initializing with a constant requires a square matrix.");

		for (unsigned i = 0; i < row; i++)
			d[i * r_dim + i] = constant;
	}

	~matrix()
	{
		delete[] d;
	}
	/*
	float *operator[](unsigned m)
	{
		return &d[m * col];
	}
	*/
	float operator()(unsigned m, unsigned n)
	{
		return d[m * col + n];
	}

	bool operator==(const matrix &other) const
	{
		return (row == other.row && col == other.col);
	}

	bool operator!=(const matrix &other) const
	{
		return !(*this == other);
	}

	//Copy assignment
	matrix<r_dim, c_dim> &
		operator=(const matrix<r_dim, c_dim> &other)
	{
		if (this != &other) // Not the same address
		{
			for (unsigned i = 0; i < row; i++)
				for (unsigned j = 0; j < col; j++)
					d[i * col + j] = other.d[i * col + j];
		}

		return *this;
	}

	//Move assignment
	matrix<r_dim, c_dim> &
		operator=(matrix<r_dim, c_dim> &&other)
	{
		if (this != &other) // Prevent self-move
		{
			delete[] d;
			d = other.d;
			other.d = nullptr;
		}

		return *this;
	}

	matrix<r_dim, r_dim> &
		operator=(const float constant)
	{
		for (unsigned i = 0; i < row; i++)
			for (unsigned j = 0; j < col; j++)
			{
				if (i == j)
					d[i * col + j] = constant;
				else
					d[i * col + j] = 0.f;
			}

		return *this;
	}

	matrix<r_dim, c_dim> &
		operator+=(const matrix<r_dim, c_dim> &other)
	{
		for (unsigned i = 0; i < row; i++)
		{
			for (unsigned j = 0; j < col; j++)
			{
				d[i * col + j] += other.d[i * col + j];
			}
		}

		return *this;
	}

	matrix<r_dim, r_dim> &
		operator+=(const float constant)
	{
		for (unsigned i = 0; i < row; i++)
			d[i * col + i] += constant;

		return *this;
	}

	matrix<r_dim, c_dim> &
		operator-=(const matrix<r_dim, c_dim> &other)
	{
		for (unsigned i = 0; i < row; i++)
		{
			for (unsigned j = 0; j < col; j++)
			{
				d[i * col + j] -= other.d[i * col + j];
			}
		}

		return *this;
	}

	matrix<r_dim, r_dim> &
		operator-=(const float constant)
	{
		for (unsigned i = 0; i < row; i++)
			d[i * col + i] -= constant;

		return *this;
	}

	/* matrix <r_dim, r_dim>&
	 * operator*= (const matrix <r_dim, r_dim>& other) {}
	 *
	 * OMITTED
	 *
	 * Matrices are not communitive under multiplication.
	 * For matrix A, A*B =/= B*A for any matrices B.
	 * Order must be clearly expressed to avoid errors.
	 * Conventions are an unnecessary risk for this abstraction.
	 */

	matrix<r_dim, r_dim> &
		operator*=(const float constant)
	{
		for (unsigned i = 0; i < row; i++)
			for (unsigned j = 0; j < col; j++)
				d[i * col + j] *= constant;

		return *this;
	}

	matrix<r_dim, c_dim> &
		operator/=(const float constant)
	{
		for (unsigned i = 0; i < row; i++)
			for (unsigned j = 0; j < col; j++)
				d[i * col + j] /= constant;

		return *this;
	}

	//May not be usefully to have
	matrix<r_dim, c_dim> &
		apply(float(*func)(float))
	{
		for (unsigned i = 0; i < row; i++)
			for (unsigned j = 0; j < col; j++)
				d[i * col + j] = (*func)(d[i * col + j]);

		return *this;
	}

	matrix<1, c_dim>
		getRow(unsigned __row) const
	{
		if (__row >= row)
			throw std::out_of_range("No such row index in the Matrix.");

		matrix<1, c_dim> __res;

		for (unsigned j = 0; j < col; j++)
		{
			__res.d[j] = (*this).d[__row * col + j];
		}

		return __res;
	}

	matrix<r_dim, 1>
		getColumn(unsigned __col) const
	{
		if (__col >= col)
			throw std::out_of_range("No such column index in the Matrix.");

		matrix<r_dim, 1> __res;

		for (unsigned i = 0; i < row; i++)
		{
			__res.d[i] = (*this).d[i * col + __col];
		}

		return __res;
	}

	void
		swapRow(unsigned row1, unsigned row2)
	{
		if (row1 == row2) return;

		if ((row1 >= row) || (row2 >= row))
			throw std::out_of_range("No such row index in the Matrix.");

		float temp;
		for (unsigned j = 0; j < col; j++)
		{
			temp = d[row1 * col + j];
			d[row1 * col + j] = d[row2 * col + j];
			d[row2 * col + j] = temp;
		}
	}

	void
		swapColumn(unsigned col1, unsigned col2)
	{
		if (col1 == col2) return;

		if ((col1 >= col) || (col2 >= col))
			throw std::out_of_range("No such column index in the Matrix.");

		float temp;
		for (unsigned i = 0; i < row; i++)
		{
			temp = d[i * col + col1];
			d[i * col + col1] = d[i * col + col2];
			d[i * col + col2] = temp;
		}
	}

	void
		linearAddRow(unsigned fromRow, unsigned toRow, float scaler)
	{
		if ((fromRow >= row) || (toRow >= row))
			throw std::out_of_range("No such row index in the Matrix.");

		for (unsigned j = 0; j < col; j++)
			d[toRow * col + j] += scaler * d[fromRow * col + j];
	}

	float
		dotProduct(const matrix<r_dim, c_dim> &other)
	{
		static_assert(r_dim == 1 || c_dim == 1, "Dot Product only defined on vectors.");

		float sum = 0;
		for (unsigned i = 0; i < r_dim * c_dim; i++)
			sum += d[i] * other.d[i];
		return sum;
	}

	auto
		crossProduct(const matrix<r_dim, c_dim> &other)
	{
		static_assert(r_dim == 1 || c_dim == 1, "Cross Product only defined on vectors.");

		if constexpr (r_dim * c_dim == 2)
		{
			return d[0] * other.d[1] - d[1] * other.d[0];
		}
		if constexpr (r_dim * c_dim == 3)
		{				
			return matrix<r_dim, c_dim> { d[1] * other.d[2] - d[2] * other.d[1], d[2] * other.d[0] - d[0] * other.d[2], d[0] * other.d[1] - d[1] * other.d[0] };
		}
	}
};

//HELPER FUNCTION
//Print the matrix in recangular form

template <unsigned r_dim, unsigned c_dim>
void print(const matrix<r_dim, c_dim> &M)
{
	for (unsigned i = 0; i < r_dim; i++)
	{
		printf("[");
		for (unsigned j = 0; j < c_dim; j++)
		{
			if (j + 1 < c_dim)
				printf("%f, ", M.d[i * c_dim + j]);
			else
				printf("%f", M.d[i * c_dim + j]);
		}
		printf("]\n");
	}
}

//HELPER FUNCTION END

//Martix addition
template <unsigned r_dim, unsigned c_dim>
matrix<r_dim, c_dim>
operator+(const matrix<r_dim, c_dim> &__lhs,
	const matrix<r_dim, c_dim> &__rhs)
{
	matrix<r_dim, c_dim> __res(__lhs);
	__res += __rhs;
	return __res;
}

template <unsigned r_dim>
matrix<r_dim>
operator+(const float &__constant,
	const matrix<r_dim> &__rhs)
{
	matrix<r_dim> __res(__constant);
	__res += __rhs;
	return __res;
}

template <unsigned r_dim>
matrix<r_dim>
operator+(const matrix<r_dim> &__lhs,
	const float &__constant)
{
	matrix<r_dim> __res(__constant);
	__res += __lhs;
	return __res;
}

//Martix subtraction
template <unsigned r_dim, unsigned c_dim>
matrix<r_dim, c_dim>
operator-(const matrix<r_dim, c_dim> &__lhs,
	const matrix<r_dim, c_dim> &__rhs)
{
	matrix<r_dim, c_dim> __res(__lhs);
	__res -= __rhs;
	return __res;
}

template <unsigned r_dim>
matrix<r_dim>
operator-(const float &__constant,
	const matrix<r_dim> &__rhs)
{
	matrix<r_dim> __res(__constant);
	__res -= __rhs;
	return __res;
}

template <unsigned r_dim>
matrix<r_dim>
operator-(const matrix<r_dim> &__lhs,
	const float &__constant)
{
	matrix<r_dim> __res(__lhs);
	__res -= matrix<r_dim>(__constant);
	return __res;
}

//Matrix multiplication
// (a x b) * (b x c) = (a x c)
// (a x b) * (c x d) = (a x d) invalid if not (b == c)
template <unsigned r1_dim, unsigned c1_dim, unsigned c2_dim>
matrix<r1_dim, c2_dim>
operator*(const matrix<r1_dim, c1_dim> &__lhs,
	const matrix<c1_dim, c2_dim> &__rhs)
{
	matrix<r1_dim, c2_dim> __res;

	for (unsigned i = 0; i < r1_dim; i++)
		for (unsigned j = 0; j < c2_dim; j++)
			for (unsigned k = 0; k < c1_dim; k++)
				__res.d[i * c2_dim + j] += __lhs.d[i * c1_dim + k] * __rhs.d[k * c2_dim + j];

	return __res;
}

template <unsigned r_dim, unsigned c_dim>
matrix<r_dim, c_dim>
operator*(const matrix<r_dim, c_dim> &__lhs,
	const float __rhs)
{
	matrix<r_dim, c_dim> __res;

	for (unsigned i = 0; i < r_dim; i++)
		for (unsigned j = 0; j < c_dim; j++)
			__res.d[i * c_dim + j] = __lhs.d[i * c_dim + j] * __rhs;

	return __res;
}

template <unsigned r_dim, unsigned c_dim>
matrix<r_dim, c_dim>
operator*(const float __lhs,
	const matrix<r_dim, c_dim> &__rhs)
{
	matrix<r_dim, c_dim> __res;

	for (unsigned i = 0; i < r_dim; i++)
		for (unsigned j = 0; j < c_dim; j++)
			__res.d[i * c_dim + j] = __lhs * __rhs.d[i * c_dim + j];

	return __res;
}

//Matrix division (by a constant)
template <unsigned r_dim, unsigned c_dim>
matrix<r_dim, c_dim>
operator/(const matrix<r_dim, c_dim> &__lhs,
	const float __rhs)
{
	matrix<r_dim, c_dim> __res;

	for (unsigned i = 0; i < r_dim; i++)
		for (unsigned j = 0; j < c_dim; j++)
			__res.d[i * c_dim + j] = __lhs.d[i * c_dim + j] / __rhs;

	return __res;
}

template <unsigned r1_dim, unsigned r2_dim, unsigned c_dim>
matrix<r1_dim + r2_dim, c_dim>
concatRows(const matrix<r1_dim, c_dim> &__M1,
	const matrix<r2_dim, c_dim> &__M2)
{
	matrix<r1_dim + r2_dim, c_dim> __res;

	for (unsigned i = 0; i < r1_dim + r2_dim; i++)
		for (unsigned j = 0; j < c_dim; j++)
		{
			if (i < r1_dim)
				__res.d[i * c_dim + j] = __M1.d[i * c_dim + j];
			else
				__res.d[i * c_dim + j] = __M2.d[(i - r1_dim) * c_dim + j];
		}

	return __res;
}

template <unsigned r_dim, unsigned c1_dim, unsigned c2_dim>
matrix<r_dim, c1_dim + c2_dim>
concatColumns(const matrix<r_dim, c1_dim> &__M1,
	const matrix<r_dim, c2_dim> &__M2)
{
	matrix<r_dim, c1_dim + c2_dim> __res;

	unsigned cols = c1_dim + c2_dim;

	for (unsigned i = 0; i < r_dim; i++)
		for (unsigned j = 0; j < cols; j++)
		{
			if (j < c1_dim)
				__res.d[i * cols + j] = __M1.d[i * c1_dim + j];
			else
				__res.d[i * cols + j] = __M2.d[i * c2_dim + j - c1_dim];
		}

	return __res;
}

//Transposition
template <unsigned r_dim, unsigned c_dim>
matrix<c_dim, r_dim>
T(const matrix<r_dim, c_dim> &__M)
{
	matrix<c_dim, r_dim> __res;

	for (unsigned i = 0; i < r_dim; i++)
		for (unsigned j = 0; j < c_dim; j++)
		{
			__res.d[j * r_dim + i] = __M.d[i * c_dim + j];
		}

	return __res;
}

//Trace
template <unsigned r_dim>
float tr(const matrix<r_dim, r_dim> &__M)
{
	float __sum = 0;

	for (unsigned i = 0; i < r_dim; i++)
		__sum += __M.d[i * r_dim + i];

	return __sum;
}

//Submatrix
//Remove a row and column
template <unsigned r_dim, unsigned c_dim>
matrix<r_dim - 1, c_dim - 1>
sub(const matrix<r_dim, c_dim> &__M, unsigned __row, unsigned __col)
{
	static_assert(r_dim > 1, "Minor requires at least two rows.");
	static_assert(c_dim > 1, "Minor requires at least two columns.");

	//TODO: Require that __row and __col are in range.
	//      OutOfRange exception!

	matrix<r_dim - 1, c_dim - 1> __res;

	unsigned row = 0;
	unsigned col = 0;

	for (unsigned i = 0; i < r_dim - 1; i++)
	{
		row = (i < __row - 1 ? 0 : 1);
		for (unsigned j = 0; j < c_dim - 1; j++)
		{
			col = (j < __col - 1 ? 0 : 1);
			__res.d[i * (r_dim - 1) + j] = __M.d[(i + row) * r_dim + (j + col)];
		}
	}

	return __res;
}

float parity(unsigned i);

//Determinant
template <unsigned r_dim>
float det(const matrix<r_dim> &__M)
{
	//printf ("det n > 2\n");
	float __res = 0;

	matrix<r_dim - 1> subM;
	float sign = 0;
	float minor = 0;
	float detV = 0;

	for (unsigned j = 0; j < r_dim; j++)
	{
		subM = sub(__M, 1, j + 1);
		sign = parity(j);
		minor = __M.d[j];
		detV = det(subM);

		__res += sign * minor * detV;
	}

	return __res;
}

template <>
float det(const matrix<2> &__M);
template <>
float det(const matrix<1> &__M);
template <>
float det(const matrix<0> &__M);

template <unsigned r_dim>
matrix<r_dim>
inverse(const matrix<r_dim> &__M)
{
	if (det(__M) == 0)
		throw;

	matrix<r_dim, 2 * r_dim> temp = concatColumns(__M, matrix<r_dim>(1.f));
	matrix<r_dim> __res;

	for (unsigned j = 0; j < r_dim; j++)
		for (unsigned i = 0; i < r_dim; i++)
		{
			if (temp(j, j) == 0.f)
			{
				if (temp(i, j) != 0.f)
				{
					temp.swapRow(i, j);
					temp.linearAddRow(j, j, 1.f / temp(j, j) - 1.f);
				}
				continue;
			}
			if (temp(j, j) != 1.f)
				temp.linearAddRow(j, j, 1.f / temp(j, j) - 1.f);

			if (i != j)
				temp.linearAddRow(j, i, -temp(i, j));
		}

	for (unsigned i = 0; i < r_dim; i++)
		for (unsigned j = 0; j < r_dim; j++)
			__res.d[i * r_dim + j] = temp(i, j + r_dim);

	return __res;
}

template <unsigned r_dim, unsigned c_dim>
matrix<r_dim, c_dim>
apply(const matrix<r_dim, c_dim> &__M, float(*func)(float)) {
	matrix<r_dim, c_dim> __res;

	for (unsigned i = 0; i < r_dim; i++)
		for (unsigned j = 0; j < c_dim; j++)
			__res.d[i * c_dim + j] = (*func)(__M.d[i * c_dim + j]);

	return __res;
}

template <unsigned r_dim, unsigned c_dim>
float
sum(const matrix<r_dim, c_dim> &__M)
{
	float res = 0;

	for (unsigned i = 0; i < r_dim; i++)
		for (unsigned j = 0; j < c_dim; j++)
			res += __M.d[i * c_dim + j];

	return res;
}