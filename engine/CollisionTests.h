#ifndef COLLISIONTESTS_H
#define COLLISIONTESTS_H

#include "MathUtil.h"
#include <algorithm>
#include <cmath>

namespace CMPUT350 {

/**
 * @brief   Tests whether two circles overlap or touch.
 * @param   a First circle.
 * @param   b Second circle.
 * @return  true if the distance between centers is at most the sum of the radii.
 * AI-ASSISTED: skeleton | AI did: explained the distance-vs-radii idea and gave the skeleton | I did: filled the blanks
 */
inline bool CircleCircle(const Circle &a, const Circle &b) {
    return a.center.Distance(b.center) <= a.radius + b.radius;
}

/**
 * @brief   Tests whether a circle overlaps or touches an axis-aligned rectangle.
 * @param   c The circle.
 * @param   r The rectangle.
 * @return  true if the point of r nearest the circle's center is within the radius.
 * AI-ASSISTED: skeleton | AI did: explained the clamp-to-nearest-point idea and gave the skeleton | I did: filled the blanks
 */
inline bool CircleRect(const Circle &c, const Rect &r) {
    float nearestX = std::clamp(c.center.x, r.topLeft.x, r.topLeft.x + r.width);   // rect's left and right edges
    float nearestY = std::clamp(c.center.y, r.topLeft.y, r.topLeft.y + r.height);   // rect's top and bottom edges
    Point2D nearest(nearestX, nearestY);
    return c.center.Distance(nearest) <= c.radius;
}

/**
 * @brief   Tests whether two axis-aligned rectangles overlap or touch.
 * @param   a First rectangle.
 * @param   b Second rectangle.
 * @return  true if they overlap (delegates to Overlaps in MathUtil.h).
 * AI-ASSISTED: skeleton | AI did: said to reuse Overlaps and gave the blank | I did: filled it
 */
inline bool RectRect(const Rect &a, const Rect &b) {
    return Overlaps(a, b);
}

/**
 * @brief   Finds where a directed line first touches a circle.
 * @param   l The line, walked from p1 toward p2.
 * @param   c The circle.
 * @param   hit Output: the first contact point. Only set when the function returns true.
 * @return  true if the segment touches the circle (including starting inside it).
 *          Solves |p1 + d*t - center|^2 = r^2 and takes the smaller root t in [0, 1].
 * AI-ASSISTED: skeleton | AI did: derived the quadratic, gave the formulas, case analysis and skeleton | I did: filled the blanks and fixed the Dot calls
 */
inline bool LineCircle(const Line &l, const Circle &c, Point2D &hit) {
    Point2D d = l.p2 - l.p1;
    Point2D f = l.p1 - c.center;
    float a = d.Dot(d);
    float b = 2 * f.Dot(d);
    float c0 = f.Dot(f) - c.radius * c.radius;
    if (c0 <= 0) { hit = l.p1; return true; }
    if (a == 0) return false;
    float disc = b * b - 4 * a * c0;
    if (disc < 0) return false;
    float t1 = (-b - std::sqrt(disc)) / (2 * a);
    if (t1 < 0 || t1 > 1) return false;
    hit = l.p1 + d * t1;
    return true;
}

/**
 * @brief   Finds where a directed line first touches an axis-aligned rectangle.
 * @param   l The line, walked from p1 toward p2.
 * @param   r The rectangle.
 * @param   hit Output: the first contact point. Only set when the function returns true.
 * @return  true if the line touches the rectangle (including starting inside it).
 * AI-ASSISTED: skeleton | AI did: explained the closest-edge-crossing approach and gave the skeleton | I did: filled the blanks
 */
inline bool LineRect(const Line &l, const Rect &r, Point2D &hit) {
    if (r.IsInside(l.p1)) {
        hit = l.p1;
        return true;
    }
    
    Point2D tl = r.topLeft;
    Point2D tr(r.topLeft.x + r.width, r.topLeft.y);
    Point2D br(r.topLeft.x + r.width, r.topLeft.y + r.height);   // bottom-right
    Point2D bl(r.topLeft.x, r.topLeft.y + r.height);   // bottom-left
    Line edges[4] = { Line(tl, tr), Line(tr, br), Line(br, bl), Line(bl, tl) };

    bool found = false;
    float best = 0;
    for (const Line &e : edges) {
        Point2D p;
        if (l.Crosses(e, p)) {
            float d = l.p1.Distance(p);
            if (!found || d < best) { best = d; hit = p; found = true; }
        }
    }
    return found;
}

}  // namespace CMPUT350

#endif  // COLLISIONTESTS_H