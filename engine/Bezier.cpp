#include "Bezier.h"

namespace CMPUT350 {

Point2D Bezier::GetPoint(float t) const {
	return GetPoint(mPoints , t);
}
Point2D Bezier::GetSlope(float t) const {
	return GetSlope(mPoints , t);
}

//AI-ASSISTED: skeleton | AI did: gave the formulas, pointed out the missing factors of 3, the 6-to-3 fix, and that t must be localT | I did: wrote the code
Point2D Bezier::GetPoint(const std::vector<Point2D> &pts, float t) {
	size_t start;
	float localT;
	FindSegment(pts, t, start, localT);
	float u = 1.0f - localT;
	return pts[start] * (u*u*u) + pts[start + 1] * (3*u*u*localT) + pts[start + 2] * (3*u*localT*localT) + pts[start + 3] * (localT*localT*localT);
}

//AI-ASSISTED: skeleton | AI did: gave the formulas, pointed out the missing factors of 3, the 6-to-3 fix, and that t must be localT | I did: wrote the code
Point2D Bezier::GetSlope(const std::vector<Point2D> &pts, float t) {
	size_t start;
	float localT;
	FindSegment(pts, t, start, localT);
	float u = 1.0f - localT;
	return ((pts[start + 1] - pts[start]) * 3 * u * u) + ((pts[start + 2] - pts[start + 1]) * 6 * u * localT) + ((pts[start + 3] - pts[start + 2]) * 3 * localT * localT);
}	

//AI-ASSISTED: skeleton | AI did: gave the structure with blanks and hints that were close to the answers; pointed out the redundant floor and the negative-t case |I did: filled the blanks
void Bezier::FindSegment(const std::vector<Point2D> &pts, float t, size_t &start, float &localT) {
    size_t numSegments = (pts.size() - 1) / 3;
    size_t seg = static_cast<size_t>(std::floor(t));
    if (seg >= numSegments) {
        seg = numSegments - 1;
        localT = 1;
    } else {
        localT = t - std::floor(t);
    }
    start = 3 * seg;
}

}
