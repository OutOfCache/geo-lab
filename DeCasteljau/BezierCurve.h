#pragma once
#include "PolynomialCurve.h"
namespace cogra
{
namespace gmca
{
template<class T>
class BezierCurve : public PolynomialCurve<T>
{
public:
    typedef T vector_type;
    typedef typename T::value_type value_type;


    BezierCurve(const std::vector<vector_type>& coefficients)
        : PolynomialCurve<T>::PolynomialCurve(coefficients)
        , m_binomialCoefficients(computeBinomialCoefficients())
    {       
    }

    /// <summary>
    /// Compute and return the bionmial coefficients for the given degree.
    /// </summary>
    /// <returns></returns>
    std::vector<value_type> computeBinomialCoefficients() const 
    { 
        std::vector<value_type> result(PolynomialCurve<T>::getOrder() - 2, 1);
        // Assignment 1(c) Implement me!
        
        const std::vector<value_type> b0 = { 1 };
        const std::vector<value_type> b1 = { 1, 1 };
        const std::vector<value_type> b2 = { 1, 2, 1 };
        const std::vector<value_type> b3 = { 1, 3, 3, 1 };
        const std::vector<value_type> b4 = { 1, 4, 6, 4, 1 };
        const std::vector<value_type> b5 = { 1, 5, 10, 10, 5, 1 };
        const std::vector<value_type> b6 = { 1, 6, 15, 20, 15, 6, 1 };
        const std::vector<value_type> b7 = { 1, 7, 21, 35, 35, 21, 7, 1 };
        /* const std::vector<value_type> b8 = {1, 8, 28,};
        const std::vector<value_type> b9 = { 1 };
        const std::vector<value_type> b10 = { 1 };*/

        // compute n!
        /*int n_fact = 0;
        for (int i = 0; i < PolynomialCurve::getOrder(); i++)
        {
            n_fact += i;
        }

        for (int j = 0; j < PolynomialCurve::getOrder(); j++)
        {
            result.push_back()
        }*/
        const std::vector<std::vector<value_type>> coeff = { b0, b1, b2, b3, b4, b5, b6 };
        result = coeff.at(PolynomialCurve<T>::getDegree());

        return result;
    }

    vector_type evaluate(value_type t) const override
    {
        // Assignment 1(d) Implement me!
        size_t n = PolynomialCurve<T>::getDegree();
        value_type v = 1 - t;
        value_type up = t;
        vector_type bi = PolynomialCurve<T>::getCoefficient(0);
        vector_type r = v * bi;
        for (ui32 i = 1; i < n; i++)
        {
            bi = PolynomialCurve<T>::getCoefficient(i);
            r = v * (r + m_binomialCoefficients.at(i) * up * bi);
            up *= t;
        }
		return r + up * PolynomialCurve<T>::getCoefficient(n);
    }

    void elevateDegree() 
    {
        // Assignment 3 Implement me!
        const auto halfway = (PolynomialCurve<T>::getDomainMin() + PolynomialCurve<T>::getDomainMax()) / 2;
        const size_t order = PolynomialCurve<T>::getOrder();
        std::vector<vector_type> controlPoints;
        controlPoints.reserve(order + 1);
        const auto deCasteljauPyramid = deCasteljau(halfway);
        const auto layer = deCasteljauPyramid.at(0);

        controlPoints.push_back(PolynomialCurve<T>::getCoefficient(0));
        controlPoints.insert(controlPoints.begin(), layer.begin(), layer.end());
        controlPoints.push_back(PolynomialCurve<T>::getCoefficient(order - 1));
    }

    std::vector<std::vector<vector_type>> deCasteljau(const std::vector<value_type>& t) const
    {
        // Assignment 2(a) Implement me!
        size_t order = PolynomialCurve<T>::getOrder();
		std::vector<std::vector<vector_type>> result;
        result.reserve(order);
        result.push_back(PolynomialCurve<T>::getCoefficients());
        order--;
        for (ui32 i = 0; i < t.size() - 1; i++)
        {
            const auto param = t.at(i);
            std::vector<vector_type> layer;
            layer.reserve(order);
            for (ui32 j = 0; j < order; j++)
            {
                const auto prevLayer = result.at(i);
                const auto b0 = prevLayer.at(j);
                const auto b1 = prevLayer.at(j + 1);
                layer.push_back((1 - param) * b0 + param * b1);
            }
            order--;
            result.push_back(layer);
        }
        
        result.erase(result.begin()); // remove original coefficients
        return result;
    }


    std::vector<std::vector<vector_type>> deCasteljau(const value_type t) const
    {
        // Assignment 2(a) Implement me!
        const size_t order = PolynomialCurve<T>::getOrder();
        std::vector<value_type> parameters(order, t);
        return deCasteljau(parameters);
    }



    std::pair<BezierCurve<vector_type>, BezierCurve<vector_type>> subdivide() const
    {
        const auto halfway = (PolynomialCurve<T>::getDomainMin() + PolynomialCurve<T>::getDomainMax()) / 2;
        const size_t order = PolynomialCurve<T>::getOrder();
        const auto deCasteljauPyramid = deCasteljau(halfway);
        std::vector<vector_type> leftCoefficients;
        leftCoefficients.reserve(order);
        std::vector<vector_type> rightCoefficients;
        rightCoefficients.reserve(order);

        leftCoefficients.push_back(PolynomialCurve<T>::getCoefficient(0));
        rightCoefficients.push_back(PolynomialCurve<T>::getCoefficient(order - 1));

        for (const auto level : deCasteljauPyramid)
        {
            leftCoefficients.push_back(level.at(0));
            rightCoefficients.push_back(level.at(level.size() - 1));
        }

        return std::pair<BezierCurve<vector_type>, BezierCurve<vector_type>>(
            BezierCurve<vector_type>(leftCoefficients),
            BezierCurve<vector_type>(rightCoefficients));
    }


private:
    std::vector<value_type>         m_binomialCoefficients;

};
}
}