#pragma once
#include <array>
#include <cmath>
#include "TVector.h"

template<size_t RowSize, size_t ColumnSize>
class TMatrix
{
protected:
	std::array<TVector<ColumnSize>, RowSize> rows_;
public:
	TMatrix()
		: rows_()
	{
	}

	TMatrix(const std::array<TVector<ColumnSize>, RowSize>& rows)
		: rows_(rows)
	{}

	TVector<ColumnSize>& operator[](int i)
	{
		return rows_[i];
	}

	TVector<ColumnSize>& operator[](size_t i)
	{
		return rows_[i];
	}

	const TVector<ColumnSize>& operator[](int i) const
	{
		return rows_[i];
	}

	const TVector<ColumnSize>& operator[](size_t i) const
	{
		return rows_[i];
	}

	const TVector<ColumnSize> getRow(size_t i) const
	{
		return rows_[i];
	}

	const TVector<ColumnSize> getRow(int i) const
	{
		return rows_[i];
	}

	const TVector<ColumnSize> getColumn(size_t i) const
	{
		TVector<RowSize> res;
		for (size_t row = 0; row < RowSize; row++)
		{
			res[row] = rows_[row][i];
		}
		return res;
	}

	const TVector<ColumnSize> getColumn(int i) const
	{
		return getColumn(static_cast<size_t>(i));
	}

	TVector<RowSize> operator*(const TVector<ColumnSize>& vec)
	{
		auto routine = [](const TVector<ColumnSize>& row, const TVector<ColumnSize>& column)
		{
			double res = 0.;
			for (size_t i = 0; i < ColumnSize; i++)
			{
				res += row[i] * column[i];
			}
			return res;
		};

		TVector<RowSize> res;

		for (size_t row = 0; row < RowSize; row++)
		{
			res[row] = routine(rows_[row], vec);
		}
		return res;
	}

	TMatrix<ColumnSize, RowSize> operator*(const TMatrix<ColumnSize, RowSize>& rhs)
	{
		auto routine = [](const TVector<ColumnSize>& row, const TVector<ColumnSize>& column)
		{
			double res = 0.;
			for (size_t i = 0; i < ColumnSize; i++)
			{
				res += row[i] * column[i];
			}
			return res;
		};

		TMatrix<ColumnSize, RowSize> res;

		for (size_t column = 0; column < RowSize; column++)
		{
			for (size_t row = 0; row < ColumnSize; row++)
			{
				res[row][column] = routine(getRow(row), rhs.getColumn(column));
			}
		}
		return res;
	}

	bool operator==(const TMatrix& rhs)
	{
		for (size_t i = 0; i < ColumnSize; i++)
		{
			for (size_t j = 0; j < RowSize; j++)
			{
				if (this[i][j] != rhs[i][j])
				{
					return false;
				}
			}
		}
		return true;
	}

	bool isNearly(const TMatrix& rhs, double eps) const
	{
		for (size_t i = 0; i < ColumnSize; i++)
		{
			if (rows_[i].isNearly(rhs.rows_[i], eps) == false)
			{
				return false;
			}
		}
		return true;
	}
};