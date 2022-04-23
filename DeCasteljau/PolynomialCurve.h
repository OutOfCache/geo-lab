#pragma once

#include "ParametricCurve.h"
#include <cogra/types.h>
#include <vector>

namespace cogra
{
namespace gmca
{
template<class T>
class PolynomialCurve : public ParametricCurve<T>
{
public:
    typedef T vector_type;
    typedef typename T::value_type value_type;
    PolynomialCurve(const std::vector< vector_type>& coefficients)
        : ParametricCurve<T>::ParametricCurve(value_type(0), value_type(1))
        , m_coefficients(coefficients)
    {}

    /// <summary>
    /// Returns the order (i.e. the number of coefficients)
    /// </summary>
    size_t getOrder() const
    {
        return m_coefficients.size();
    }

    /// <summary>
    /// Returns the degree of the curve.
    /// </summary>
    size_t getDegree() const
    {
        return getOrder() - 1;
    }

    /// <summary>
    /// Returns a coefficient;
    /// </summary>
    /// <param name="index">Index to the coefficient.</param>
    const vector_type& getCoefficient(size_t index) const
    {
        return m_coefficients[index];
    }


    const std::vector<vector_type>& getCoefficients() const
    {
        return m_coefficients;
    }

    std::vector<vector_type>& getCoefficients()
    {
        return m_coefficients;
    }


private:
    std::vector<vector_type>    m_coefficients;
};
}
}
