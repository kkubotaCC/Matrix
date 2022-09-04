#pragma once
#include <array>
#include <algorithm>
#include <cmath>

template<size_t T>
class TVector
{
	std::array<double, T> elements_;
public:
	TVector()
		: elements_({ 0 })
	{
	}

	TVector(const std::array<double, T>& elements)
		: elements_(elements)
	{
	}

	double& operator[](int i)
	{
		return elements_[i];
	}

	double& operator[](size_t i)
	{
		return elements_[i];
	}

	const double& operator[](int i) const
	{
		return elements_[i];
	}

	const double& operator[](size_t i) const
	{
		return elements_[i];
	}

	double length2() const
	{
		double res = 0;
		std::for_each(elements_.begin(), elements_.end(), [&res](const double e) { res += e * e; });
		return res;
	}

	double length() const
	{
		return std::sqrt(length2());
	}

	bool isNearly(const TVector& rhs, double eps) const
	{
		for (size_t i = 0; i < T; i++)
		{
			if (std::abs((*this)[i] - rhs[i]) > eps)
			{
				return false;
			}
		}
		return true;
	}
};