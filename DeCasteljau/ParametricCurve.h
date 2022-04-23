#pragma once

#include <cogra/types.h>
#include <vector>
#include <cogra/exceptions/RuntimeError.h>
namespace cogra
{
namespace gmca
{
/// <summary>
/// A class for parameteric curves.
/// </summary>
template<class T>
class ParametricCurve
{
public:
    typedef T vector_type;
    typedef typename T::value_type value_type;

    /// <summary>
    /// Creates a 2D parametric curve with 1D domain.
    /// </summary>
    /// <param name="domainMin">Lower bound of the domain.</param>
    /// <param name="domainMax">Upper bound of the domain. Must greater the domainMin or otherwise an exception is thrown.</param>
    ParametricCurve(value_type domainMin, value_type domainMax) 
        : m_domainMin(domainMin)
        , m_domainMax(domainMax)
    {
        if(domainMax <= domainMin)
        {
            throw cogra::exceptions::RuntimeError("domainMin must be smaller than domainMax");
        }
    }
    
    /// <summary>
    /// Evaluates the curve at parameter. Override this method in a subclass to specify the particular curve type.
    /// </summary>
    /// <param name="t">The parameter along the parameter domain.</param>
    /// <returns>A 2D point on the curve.</returns>
    virtual vector_type evaluate(value_type t) const = 0;

    /// <summary>
    /// Samples the parameter domain with nSamplePoints. 
    /// 
    /// We distribute the sample points uniformly across the parameter domain.
    /// For each sample, the method evaluate is called. Implement evaluate in a derived class of this class.
    /// </summary>
    /// <param name="nSamplePoints">Number of sample points.</param>
    /// <returns>A vector with nSamplePoints distribute over the curve. The points are order by increasing parameter value values.</returns>
    virtual std::vector<vector_type> sample(size_t nSamplePoints) const
    {
        // Assignment 1(b) Implement me!
        std::vector<vector_type> sampledPoints;
        sampledPoints.reserve(nSamplePoints);

        const auto interval = (m_domainMax - m_domainMin) / ( nSamplePoints - 1);
        for (int i = 0; i < nSamplePoints; i++)
        {
            sampledPoints.push_back(evaluate(m_domainMin + (i * interval)));
        }
        return sampledPoints;
    }

    /// <summary>
    /// Getter function.
    /// </summary>
    /// <returns>The lower bound of the domain.</returns>
    value_type getDomainMin() const
    {
        return m_domainMin;
    }

    /// <summary>
    /// Getter function.
    /// </summary>
    /// <returns>The upper bound of the domain.</returns>
    value_type getDomainMax() const
    {
        return m_domainMax;
    }

private:
    value_type             m_domainMin;

    value_type             m_domainMax;
};

}
}