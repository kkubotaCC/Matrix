
#include "pch.h"
#include "CppUnitTest.h"

#include "../Matrix/Matrix33.h"
#include "../Matrix/Matrix23.h"
#include "../Matrix/TRSMatrix.h"

#include <random>
#include <cmath>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace MatrixTest
{
	constexpr double eps() { return 1e-7; }

	template<typename T>
	bool isNealy(T lhs, T rhs, T eps)
	{
		return (std::abs(lhs - rhs) <= eps);
	}

	template<int RowSize, int ColumnSize>
	TMatrix<RowSize, ColumnSize> RandMatrix()
	{
		std::random_device seed_gen;
		std::mt19937 engine(seed_gen());

		std::uniform_real_distribution<> dist(-10., 10.);

		TMatrix<RowSize, ColumnSize> res;

		for (size_t i = 0; i < RowSize; i++)
		{
			for (size_t j = 0; j < ColumnSize; j++)
			{
				res[i][j] = dist(engine);
			}
		}
		return res;
	}

	template<int T>
	TVector<T> RandVec()
	{
		std::random_device seed_gen;
		std::mt19937 engine(seed_gen());

		std::uniform_real_distribution<> dist(-100000., 100000.);

		TVector<T> res;
		for (size_t i = 0; i < T; i++)
		{
			res[i] = dist(engine);
		}
		return res;
	}

	TEST_CLASS(TSquareMatrixTest)
	{
	public:

		TEST_METHOD(ScaleTest)
		{
			double scaleX = 10.;
			double scaleY = 20.;
			auto scale = Matrix33::Scale(TVector<2>({ scaleX, scaleY }));

			for (size_t row = 0; row < 3; row++)
			{
				for (size_t column = 0; column < 3; column++)
				{
					if (row == 0 && column == 0)
					{
						Assert::AreEqual(scale[row][column], scaleX);
					}
					else if (row == 1 && column == 1)
					{
						Assert::AreEqual(scale[row][column], scaleY);
					}
					else if (row == 2 && column == 2)
					{
						Assert::AreEqual(scale[row][column], 1.);
					}
					else
					{
						Assert::AreEqual(scale[row][column], 0.);
					}
				}
			}
		}
		TEST_METHOD(TranslateTest)
		{
			double translateX = 10.;
			double translateY = 20.;
			auto trans = Matrix33::Translate(TVector<2>({ translateX, translateY }));

			auto pos = TVector<3>({ 1, 1, 1 });

			for (size_t row = 0; row < 3; row++)
			{
				for (size_t column = 0; column < 2; column++)
				{
					if (column == row)
					{
						Assert::AreEqual(trans[row][column], 1.);
					}
					else if (column == 2 && row == 0)
					{
						Assert::AreEqual(trans[row][column], translateX);
					}
					else if (column == 2 && row == 1)
					{
						Assert::AreEqual(trans[row][column], translateY);
					}
					else
					{
						Assert::AreEqual(trans[row][column], 0.);
					}
				}
			}
		}
	};

	TEST_CLASS(TMatrixTest)
	{
	public:
		TEST_METHOD(MultiTest_WitVec)
		{
			{
				for (size_t i = 0; i < 100; i++)
				{
					auto vec = RandVec<3>();

					auto scale = Matrix33::Scale({ {100, 200} });

					auto res = scale * vec;

					Assert::IsTrue(isNealy(res[0], vec[0] * scale[0][0], eps()));
					Assert::IsTrue(isNealy(res[1], vec[1] * scale[1][1], eps()));
				}
			}
			{
				auto vec = TVector<3>({ 1, 1, 1 });

				auto trans = Matrix33::Translate({ {100, 200} });

				auto res = trans * vec;

				Assert::IsTrue(isNealy(res[0], vec[0] + trans[0][2], eps()));
				Assert::IsTrue(isNealy(res[1], vec[1] + trans[1][2], eps()));
			}
		}
		TEST_METHOD(MultiTest_WithMatrix)
		{
			std::random_device seed_gen;
			std::mt19937 engine(seed_gen());

			std::uniform_real_distribution<> dist(-10., 10.);
			for(int i = 0; i < 100; i++)
			{
				auto scale = Matrix33::Scale({ {dist(engine), dist(engine)}});
				auto trans = Matrix33::Translate({ {dist(engine), dist(engine)} });

				auto res = trans * scale;

				for (size_t i = 0; i < 3; i++)
				{
					for (size_t j = 0; j < 3; j++)
					{
						if (i == j)
						{
							Assert::IsTrue(isNealy(res[i][j], scale[i][j], eps()));
						}
						else if (j == 2)
						{
							Assert::IsTrue(isNealy(res[i][j], trans[i][j], eps()));
						}
						else
						{
							Assert::IsTrue(isNealy(res[i][j], 0., eps()));
						}
					}
				}
			}
			for (int i = 0; i < 100; i++)
			{
				auto mat1 = RandMatrix<3, 3>();
				auto mat2 = RandMatrix<3, 3>();

				auto vec = RandVec<3>();

				auto mat12 = mat1 * mat2;

				auto res = mat2 * vec;
				res = mat1 * res;

				Assert::IsTrue(res.isNearly(mat12 * vec, eps()));
			}
		}
	};
	TEST_CLASS(TRSMatrixTest)
	{
	public:

		TEST_METHOD(TestMethod1)
		{
			constexpr double pi = 3.14;
			std::random_device seed_gen;
			std::mt19937 engine(seed_gen());

			std::uniform_real_distribution<> scaledist(0.1, 10.);
			std::uniform_real_distribution<> transdist(-10000., 10000.);
			std::uniform_real_distribution<> rotetedist(-pi, pi);
			for (int i = 0; i < 100; i++)
			{
				auto scale = Matrix33::Scale({ { scaledist(engine), scaledist(engine)}});
				auto translate = Matrix33::Translate({ { transdist(engine), transdist(engine)}});
				auto rotate = Matrix33::Rotate(rotetedist(engine));

				auto mat = translate * rotate * scale;

				Matrix23 mat23;
				mat23[0] = mat[0];
				mat23[1] = mat[1];

				TRSMatrix trs(mat23);

				auto trsScale = trs.getScaleMatrix();
				auto trsTranse = trs.getTranslateMatrix();
				auto trsRotate = trs.getRotateMatrix();

				Assert::IsTrue(scale.isNearly(trsScale, eps()));
				Assert::IsTrue(translate.isNearly(trsTranse, eps()));
				Assert::IsTrue(rotate.isNearly(trsRotate, eps()));
			}
		}
	};
}
