#pragma once
#include "TSquareMatrix.h"
#include "Matrix23.h"
#include "Matrix33.h"

class TRSMatrix
{
	Matrix33 t_;
	Matrix33 r_;
	Matrix33 s_;
public:

	TRSMatrix(const Matrix23& mat)
	{
		setup(mat);
	}

	const Matrix33& getScaleMatrix() const
	{
		return s_;
	}

	const Matrix33& getRotateMatrix() const
	{
		return r_;
	}

	const Matrix33& getTranslateMatrix() const
	{
		return t_;
	}

	void setup(const Matrix23& mat)
	{
		auto mat33 = mat.toMatrix33();

		double scaleX = mat33.getColumn(0).length();
		double scaleY = mat33.getColumn(1).length();

		double translateX = mat33[0][2];
		double translateY = mat33[1][2];

		s_ = Matrix33::Scale(TVector<2>({ scaleX, scaleY }));
		t_ = Matrix33::Translate(TVector<2>({ translateX, translateY }));

		r_ = Matrix33(
				std::array<TVector<3>, 3>(
					{
						TVector<3>({mat[0][0] / scaleX, mat[0][1] / scaleY,0}),
						TVector<3>({mat[1][0] / scaleX, mat[1][1] / scaleY,0}),
						TVector<3>({ 0, 0, 1})
					}
			)
		);
	}
};

