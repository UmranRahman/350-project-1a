#ifndef BEZIER_H
#define BEZIER_H

#include "MathUtil.h"
#include <vector>
#include <cmath>

namespace CMPUT350 {

class Bezier {
public:
	Bezier(const std::vector<Point2D> &pts): mPoints(pts) {}
	
	Point2D GetPoint(float t) const;
	Point2D GetSlope(float t) const;
	
	static Point2D GetPoint(const std::vector<Point2D> &pts, float t);
	static Point2D GetSlope(const std::vector<Point2D> &pts, float t);	

private:
    std::vector<Point2D> mPoints;

    static void FindSegment(const std::vector<Point2D> &pts, float t, size_t &start, float &localT);
};
} // namespace CMPUT350
#endif // BEZIER_H
