#pragma once
#include <array>
#include <cmath>
#include "TMatrix.h"

template<int T>
class TSquareMatrix
	:public TMatrix<T, T>
{
public:
	using _Base = TMatrix<T, T>;
	using _Base::TMatrix;

	static TSquareMatrix Scale(TVector<T-1> scale)
	{
		TSquareMatrix res;
		for (size_t i = 0; i < T-1; i++)
		{
			res[i][i] = scale[i];
		}
		res[T - 1][T - 1] = 1;
		return res;
	}

	static TSquareMatrix Translate(TVector<T - 1> translate)
	{
		TSquareMatrix res;
		for (size_t i = 0; i < T - 1; i++)
		{
			res[i][T-1] = translate[i];
		}
		for (size_t i = 0; i < T; i++)
		{
			res[i][i] = 1;
		}
		return res;
	}

	static TSquareMatrix<T> Rotate(double rad);
};

using Matrix44 = TSquareMatrix<4>;

using Matrix33 = TSquareMatrix<3>;

template<>
inline Matrix33 Matrix33::Rotate(double rad)
{
	return Matrix33(
		std::array<TVector<3>, 3>
		({
			TVector<3>({ std::cos(rad), -std::sin(rad), 0}),
			TVector<3>({ std::sin(rad),  std::cos(rad), 0}),
			TVector<3>({             0,              0, 1})
			})
	);
}