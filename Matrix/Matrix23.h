#pragma once
#include "TMatrix.h"
#include "Matrix33.h"

class Matrix23
	: public TMatrix<2, 3>
{
public:
	using TMatrix::TMatrix;

	Matrix33 toMatrix33() const
	{
		return Matrix33(
			std::array<TVector<3>, 3>(
			{
				rows_[0],
				rows_[1],
				TVector<3>({ 0, 0, 1 }),
			})
		);
	}
	
};

