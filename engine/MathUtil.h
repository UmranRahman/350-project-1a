#ifndef MATHUTIL_H
#define MATHUTIL_H

#include <cmath>
#include <iostream>
#include <algorithm> //AI-assisted: so it works in linux machines

namespace CMPUT350 {

struct Point2D {
    float x, y;
    Point2D(float x = 0, float y = 0) : x(x), y(y) {}
    float Distance(const Point2D &other) const {
        // TODO: write this code
        float x2 = x - other.x;
        float y2 = y - other.y;
        double dist = sqrt(x2*x2 + y2*y2);
        return dist;
    }
    Point2D operator+(const Point2D &other) const {
        // TODO: write this code
        return Point2D(x+other.x, y+other.y);
    }
    Point2D operator+(const float &other) const {
        // TODO: write this code
        return Point2D(x+other, y+other);
    }
    Point2D operator-(const Point2D &other) const {
        // TODO: write this code
        return Point2D(x-other.x, y-other.y);
    }
    Point2D operator-(const float &other) const {
        // TODO: write this code
        return Point2D(x-other, y-other);
    }
    Point2D operator*(const float &scalar) const {
        // TODO: write this code
        return Point2D(x*scalar, y*scalar);
    }
    Point2D &operator+=(const float &scalar) {
        // TODO: write this code
        x += scalar;
        y += scalar;
        return *this;
    }
    Point2D &operator+=(const Point2D &other) {
        // TODO: write this code
        x += other.x;
        y += other.y;
        return *this;
    }
    Point2D &operator-=(const Point2D &other) {
        // TODO: write this code
        x -= other.x;
        y -= other.y;
        return *this;
    }
    bool operator==(const Point2D &other) const {
        // TODO: write this code
        if (x == other.x && y == other.y){
            return true;
        } else {
            return false;
        }
    }
    Point2D &operator*=(const float &scalar) {
        // TODO: write this code
        x *= scalar;
        y *= scalar;
        return *this;
    }
    Point2D &operator/=(const float &scalar) {
        // TODO: write this code
        x /= scalar;
        y /= scalar;
        return *this;
    }
    float operator*(const Point2D &other) const {
        // TODO: write this code
        return x*other.x + y*other.y;
    }
    float Dot(Point2D b) const {
        // TODO: write this code
        return x*b.x + y*b.y;
    }

    /**
    * @brief   2D cross product of this vector with b (a single number).
    * @param   b The second vector.
    * @return  x*b.y - y*b.x. Zero when the vectors are parallel.
    * AI-ASSISTED: explained | AI did: pointed out the handout lists a member Cross | I did: wrote it
    */
    float Cross(Point2D b) const {
       return x * b.y - y * b.x;
    }

    static float Dot(Point2D a, Point2D b) {
        // TODO: write this code
        return a.x*b.x + a.y*b.y;
    }
    static float Cross(Point2D a, Point2D b) {
        // TODO: write this code
        return a.x*b.y - a.y*b.x;
    }
    void Normalize() {
        // TODO: write this code
        // AI-assisted: guard against zero-length vectors (see README)
        float len = sqrt(x*x + y*y);
        if (len > 0){
            x /= len;
            y /= len;
        }
    }
};

inline std::ostream &operator<<(std::ostream &os, const Point2D &p) {
    // TODO: write this code
    return os << "(" << p.x << ","<< p.y << ")";
}

inline Point2D operator*(float number, const Point2D &rhs) {
    // TODO: write this code
    return rhs * number;
}

struct Line {
    Point2D p1, p2;

    Line(Point2D p1 = {0, 0}, Point2D p2 = {0, 0}) : p1(p1), p2(p2) {}
    Line(float x1, float y1, float x2, float y2) : p1(x1, y1), p2(x2, y2) {}
    
    /**
     * @brief   Length of the segment from p1 to p2.
     * @return  Distance between the endpoints.
     * AI-ASSISTED: explained | AI did: pointed out that Distance takes the other point as an argument | I did: wrote it
     */
    float Length() const {
        // TODO: write this code
        return p1.Distance(p2);
    }

    /**
     * @brief   Finds the point on this segment closest to p.
     * @param   p The point to measure from.
     * @return  The nearest point on the segment, clamped to p1 or p2 when the perpendicular falls outside it.
     * AI-ASSISTED: skeleton | AI did: explained the projection idea (lecture 6) with a worked table and a skeleton with
     *              blanks, and suggested the zero-length guard | I did: wrote the code
     */
    Point2D ClosestPoint(const Point2D &p) const {
        // TODO: write this code
        Point2D ab = p2 - p1;
        Point2D ap = p - p1; 
        float abDot = ab.Dot(ab);
        if (abDot == 0.0) return p1;
        float t = ab.Dot(ap)/abDot;
        if (t < 0) t = 0;
        if (t > 1) t = 1;
        return (p1 + ab*t);
    }

    /**
     * @brief   Tests whether this segment crosses another, and where.
     * @param   other The second segment.
     * @param   crossingPoint Output: set to the intersection point only when the function returns true.
     * @return  true if the segments cross within both of their lengths; false if they miss or are parallel.
     * AI-ASSISTED: skeleton | AI did: gave the t/u formulas (lecture 6) as a skeleton and pointed out three bugs in my
     *              first version (wrong vector in u, crossingPoint never set properly, exact comparison to zero) |
     *              I did: wrote it and fixed them
     */
    bool Crosses(const Line &other, Point2D &crossingPoint) const {
        // TODO: write this code
        Point2D P = p2 - p1;
        Point2D Q = other.p2 - other.p1;
        float crossPQ = Point2D::Cross(P, Q);

        if (std::fabs(crossPQ) < 0.000001f) return false;       // parallel, AI suggested to avoid errors

        float t = Point2D::Cross((other.p1 - p1), Q) / crossPQ;
        float u = Point2D::Cross((other.p1 - p1), P) / crossPQ;
        if (t >= 0 && t <= 1 && u >= 0 && u <= 1) {
            crossingPoint = p1 + P * t;                           // only set on a hit
            return true;
        }
        return false;
    }
};

// AI-ASSISTED: explained | AI did: noted 1b requires the Circle printer and that Line's was an empty stub |
//              I did: wrote both
inline std::ostream &operator<<(std::ostream &os, const Line &l) {
    // TODO: write this code
    return os << l.p1 << " -> " << l.p2;
}

struct Circle {
    Point2D center;
    float radius;

    Circle(Point2D c = {0, 0}, float r = 0) : center(c), radius(r) {}

    Circle(float x, float y, float r) : center(x, y), radius(r) {}
};

// AI-ASSISTED: explained | AI did: noted 1b requires the Circle printer and that Line's was an empty stub |
//              I did: wrote both
inline std::ostream &operator<<(std::ostream &os, const Circle &c) {
    return os << c.center << " r=" << c.radius;
}

struct Rect {
    Point2D topLeft;
    float width, height;

    Rect(float left, float top, float width, float height)
        : topLeft(Point2D(left, top)), width(width), height(height) {}

    Rect(Point2D tl = {0, 0}, float w = 0, float h = 0) : topLeft(tl), width(w), height(h) {}

    // Creates bounding box around p1 and p2 with positive width/height
    Rect(Point2D p1, Point2D p2)
        : topLeft(std::min(p1.x, p2.x), std::min(p1.y, p2.y)),
        width(fabs(p1.x - p2.x)),
        height(fabs(p1.y - p2.y)) {}

    Rect(Point2D center, float radius)
        : topLeft(center.x - radius, center.y - radius), width(2 * radius), height(2 * radius) {}

    Rect &operator|=(const Rect &other) {
        // TODO: write this code
        float left = std::min(topLeft.x, other.topLeft.x);
        float top = std::min(topLeft.y, other.topLeft.y);
        float right = std::max(topLeft.x + width, other.topLeft.x + other.width);
        float bottom = std::max(topLeft.y + height, other.topLeft.y + other.height);

        topLeft = Point2D(left, top);
        width = right - left;
        height = bottom - top;
        return *this;
    }
    Rect &operator|=(const Point2D &other) {
        // TODO: write this code
        Rect rect = Rect(other, other);
        float left = std::min(topLeft.x, rect.topLeft.x);
        float top = std::min(topLeft.y, rect.topLeft.y);
        float right = std::max(topLeft.x + width, rect.topLeft.x + rect.width);
        float bottom = std::max(topLeft.y + height, rect.topLeft.y + rect.height);

        topLeft = Point2D(left, top);
        width = right - left;
        height = bottom - top;
        return *this;
    }
    Rect &operator|=(const Line &other) {
        // TODO: write this code
        Rect rect = Rect(other.p1, other.p2);
        float left = std::min(topLeft.x, rect.topLeft.x);
        float top = std::min(topLeft.y, rect.topLeft.y);
        float right = std::max(topLeft.x + width, rect.topLeft.x + rect.width);
        float bottom = std::max(topLeft.y + height, rect.topLeft.y + rect.height);

        topLeft = Point2D(left, top);
        width = right - left;
        height = bottom - top;
        return *this;
    }
    Rect &operator&=(const Rect &other) {
        // TODO: write this code
        float left = std::max(topLeft.x, other.topLeft.x);
        float top = std::max(topLeft.y, other.topLeft.y);
        float right = std::min(topLeft.x + width, other.topLeft.x + other.width);
        float bottom = std::min(topLeft.y + height, other.topLeft.y + other.height);
        if (right < left || bottom < top) {
            width = 0;
            height = 0;
        } else {
            topLeft = Point2D(left, top);
            width = right - left;
            height = bottom - top;
        }
        return *this;
    }
    Rect &operator+=(const Point2D &other) {
        // TODO: write this code
        //move rectangle
        topLeft += other;
        return *this;
    }
    Rect operator+(const Point2D &other) const {
        // TODO: write this code
        // copy rectangle to new position
        Rect rect = *this;
        rect.topLeft += other;
        return rect;
    }
    void Inset(int inset) {
        // TODO: write this code
        //shrink on all sides
        topLeft += float(inset);
        width -= 2*inset;
        height -= 2*inset;
        if (width < 0) {width = 0;}
        if (height < 0) {height = 0;}
    }
    bool IsInside(const Point2D &p) const {
        // TODO: write this code
        return p.x >= topLeft.x && p.x <= topLeft.x + width && p.y >= topLeft.y && p.y <= topLeft.y + height;
    }
};

inline std::ostream &operator<<(std::ostream &os, const Rect &l) {
    // TODO: write this code
    return os << l.topLeft << " " << l.width << " " << l.height;
}

// AI-assisted: AABB overlap test based on the Lecture 6 "prove no gap" method
inline bool Overlaps(const Rect &a, const Rect &b) {
    bool noOverlap = (a.topLeft.x + a.width  < b.topLeft.x) ||   // a is entirely left of b
                     (b.topLeft.x + b.width  < a.topLeft.x) ||   // b is entirely left of a
                     (a.topLeft.y + a.height < b.topLeft.y) ||   // a is entirely above b
                     (b.topLeft.y + b.height < a.topLeft.y);     // b is entirely above a
    return !noOverlap;
}

}  // namespace CMPUT350

#endif  // MATHUTIL_H
