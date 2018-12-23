#include "matrix.h"

double parity(unsigned i)
{
	return (i % 2 ? -1.f : 1.f);
}

template <>
double det(const matrix<2> &__M)
{
	return __M.d[0] * __M.d[3] -
		__M.d[1] * __M.d[2];
}
template <>
double det(const matrix<1> &__M)
{
	return __M.d[0];
}
template <>
double det(const matrix<0> &__M)
{
	return 0.f;
}

int main()
{
	return 0;
}