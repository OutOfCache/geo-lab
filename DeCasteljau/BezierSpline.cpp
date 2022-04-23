#include "BezierSpline.h"
namespace cogra::gmca
{
 BezierSpline::BezierSpline()
{
	std::vector<f32vec2> controlPoints = {
		f32vec2(-0.6f, -0.5f),
		f32vec2(-0.3f, 0.6f),
		f32vec2(0.3f, 0.6f),
		f32vec2(0.7f, 0.2f),
	};
	m_curves.emplace_back(controlPoints);
}

 void BezierSpline::subdivide(uint32 curveIdx)
{
	auto result = m_curves[curveIdx].subdivide();
	m_curves[curveIdx] = result.first;
	m_curves.push_back(result.second);
}

 uint32 BezierSpline::getNumberOfCurves() const
{
	return static_cast<uint32>(m_curves.size());
}

}