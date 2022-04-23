#pragma once
#include <cogra/types.h>
#include <vector>
#include "BezierCurve.h"
namespace cogra::gmca
{
class BezierSpline
{
public:
	BezierSpline();

	void subdivide(uint32 curveIdx);	

	uint32 getNumberOfCurves() const;

	std::vector<BezierCurve<f32vec2>> m_curves;
};
}