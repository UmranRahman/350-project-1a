#include "MathUtil.h"

using namespace CMPUT350;
int main() {
    Rect a(Point2D(0,0), 10, 10), b(Point2D(5,5), 10, 10), c(Point2D(50,50), 10, 10);
    std::cout << Overlaps(a, b) << Overlaps(a, c) << "\n";                  // 10
    Rect u = a; u |= b;  std::cout << u << "\n";                            // (0,0) 15 15
    Rect i = a; i &= b;  std::cout << i << "\n";                            // (5,5) 5 5
    std::cout << a.IsInside(Point2D(3,3)) << a.IsInside(Point2D(20,3)) << "\n";   // 10
    std::cout << Point2D(1,2).Distance(Point2D(4,6)) << "\n";               // 5
    std::cout << 2.0f * Point2D(1,2) << "\n";                               // (2,4)
    std::cout << Rect(Point2D(100,100), 15.0f).topLeft << "\n";             // (85,85)
}