#include "DrawContext.h"


// AI-assisted: SFML 3 usage (Text constructor taking a font, outline setters, ConvexShape)
// was learned from AI-provided examples; the functions below are my own adaptations. See README.
namespace CMPUT350 {

DrawContext::DrawContext(std::shared_ptr<sf::RenderWindow> window, std::shared_ptr<sf::Font> font)
    : mWindow(window), mFont(font) {}

void DrawContext::DrawCenteredText(const std::string &text, int pixelSize, Point2D p, RGBColor c) {
    Point2D position = Transform(p);
    sf::Text label(*mFont);
    label.setString(text);
    label.setCharacterSize(static_cast<unsigned int>(pixelSize));
    label.setFillColor(sf::Color(c.r, c.g, c.b));
    label.setPosition({position.x, position.y});
    mWindow->draw(label);
}

void DrawContext::DrawText(const std::string &text, int pixelSize, Point2D p, RGBColor c) {
    Point2D position = Transform(p);
    sf::Text label(*mFont);
    label.setString(text);
    label.setCharacterSize(static_cast<unsigned int>(pixelSize));
    label.setFillColor(sf::Color(c.r, c.g, c.b));
    label.setPosition({position.x, position.y});
    mWindow->draw(label);
}

/**
 * @brief   Draws a filled circle at a point given in the object's local space.
 * @param   p Center in local coordinates (transformed to screen space).
 * @param   radius Radius in pixels (unchanged by the transform).
 * @param   c Fill color.
 * AI-ASSISTED: skeleton | AI did: said to transform the center only and gave the blank | I did: filled the blank and wrote the rest
 */
void DrawContext::DrawCircle(Point2D p, float radius, RGBColor c) {
    Point2D center = Transform(p);
    sf::CircleShape circle(radius);
    //sf::Vector2f position({p.x, p.y});
    sf::Vector2f position({center.x, center.y});
    sf::Vector2f origin(radius, radius);
    circle.setOrigin(origin);
    circle.setPosition(position);
    circle.setFillColor(sf::Color(c.r, c.g, c.b));
    mWindow->draw(circle);
    
    
}

//AI-ASSISTED: skeleton | AI did: gave the four-corner ConvexShape structure with blanks | I did: filled the blanks
void DrawContext::DrawRect(Rect r, RGBColor c) {
    // corners in order around the shape, so the polygon doesn't twist
    Point2D tl = Transform(r.topLeft);
    Point2D tr = Transform(Point2D(r.topLeft.x + r.width, r.topLeft.y));
    Point2D br = Transform(Point2D(r.topLeft.x + r.width, r.topLeft.y + r.height));   // bottom-right: left + width, top + height
    Point2D bl = Transform(Point2D(r.topLeft.x, r.topLeft.y + r.height));   // bottom-left: left, top + height

    sf::ConvexShape shape(4);
    shape.setPoint(0, {tl.x, tl.y});
    shape.setPoint(1, {tr.x, tr.y});
    shape.setPoint(2, {br.x, br.y});           // which corner comes third?
    shape.setPoint(3, {bl.x, bl.y});           // and fourth?
    shape.setFillColor(sf::Color(c.r, c.g, c.b));
    mWindow->draw(shape);
}
/*
void DrawContext::DrawRect(Rect r, RGBColor c) {
    sf::RectangleShape rect({r.width, r.height});
    rect.setPosition({r.topLeft.x, r.topLeft.y});
    rect.setFillColor(sf::Color(c.r, c.g, c.b));
    mWindow->draw(rect);
}*/


//AI-ASSISTED: skeleton | AI did: gave the four-corner ConvexShape structure with blanks | I did: filled the blanks
void DrawContext::FrameRect(Rect r, float width , RGBColor c) {
    Point2D tl = Transform(r.topLeft);
    Point2D tr = Transform(Point2D(r.topLeft.x + r.width, r.topLeft.y));
    Point2D br = Transform(Point2D(r.topLeft.x + r.width, r.topLeft.y + r.height));
    Point2D bl = Transform(Point2D(r.topLeft.x, r.topLeft.y + r.height));

    sf::ConvexShape shape(4);
    shape.setPoint(0, {tl.x, tl.y});
    shape.setPoint(1, {tr.x, tr.y});
    shape.setPoint(2, {br.x, br.y});
    shape.setPoint(3, {bl.x, bl.y});
    shape.setFillColor(sf::Color::Transparent);
    shape.setOutlineThickness(width);
    shape.setOutlineColor(sf::Color(c.r, c.g, c.b));
    mWindow->draw(shape);
}
/*
void DrawContext::FrameRect(Rect r, float width, RGBColor c) {
    sf::RectangleShape rect({r.width, r.height});
    rect.setPosition({r.topLeft.x, r.topLeft.y});
    rect.setOutlineThickness(width);
    rect.setOutlineColor(sf::Color(c.r, c.g, c.b));
    rect.setFillColor(sf::Color::Transparent);
    mWindow->draw(rect);
}*/

/**
 * @brief Draws a line between two points with a specified width and color.
 *
 * @param from The starting point of the line (Point2D).
 * @param to The ending point of the line (Point2D).
 * @param width The width of the line in pixels.
 * @param c The color of the line, specified as an RGBColor object.
 *
 * AI-ASSISTED: skeleton | AI did: said to transform both endpoints and use them for direction and corners, gave the blanks | I did: filled the blanks and wrote the rest
 */
void DrawContext::DrawLine(Point2D from, Point2D to, float width, RGBColor c) {
    Point2D a = Transform(from);
    Point2D b = Transform(to);
    //Point2D direction = to - from;
    Point2D direction = b - a;
    direction.Normalize();
    //AI-Assisted: the perpendicular-offset idea for a thick line (rotate direction 90 degrees, scale by half the width) was explained by AI; I wrote the corner code. See README.
    Point2D perp(-direction.y, direction.x);
    Point2D offset = perp * (width / 2);

    sf::ConvexShape shape(4);

    Point2D p1 = a + offset;
    shape.setPoint(0, {p1.x, p1.y});
    Point2D p2 = b + offset;
    shape.setPoint(1, {p2.x, p2.y});
    Point2D p3 = b - offset;
    shape.setPoint(2, {p3.x, p3.y});
    Point2D p4 = a - offset;
    shape.setPoint(3, {p4.x, p4.y});
    shape.setFillColor(sf::Color(c.r, c.g, c.b));
    mWindow->draw(shape);
}

int DrawContext::GetWindowWidth() { return mWindow->getSize().x; }

int DrawContext::GetWindowHeight() { return mWindow->getSize().y; }

/**
 * @brief   Converts a point from the object's local space to screen space.
 * @param   p A point in local coordinates.
 * @return  p rotated by mRotation about the local origin, then shifted by mOffset (p' = M*p + o, Lecture 7 4.2).
 * AI-ASSISTED: explained | AI did: pointed to Lecture 7 4.2, the rotate-then-shift order, and the literal-1 and missing-offset bugs; wrote this comment text | I did: wrote the function
 */
Point2D DrawContext::Transform(Point2D p) {
    float pXPrime = std::cos(mRotation)*p.x - std::sin(mRotation)*p.y + mOffset.x;
    float pYPrime = std::sin(mRotation)*p.x + std::cos(mRotation)*p.y + mOffset.y ;
    return Point2D(pXPrime, pYPrime);
}

/**
 * @brief   Converts a point from screen space back to the object's local space.
 * @param   p A point in screen coordinates.
 * @return  The local point q such that Transform(q) == p. Undoes Transform in reverse order:
 *          subtract mOffset, then rotate by -mRotation (Lecture 7, section 6.1).
 * AI-ASSISTED: skeleton | AI did: gave the two-step structure with blanks; wrote this comment text | I did: filled the blanks
 */
Point2D DrawContext::ReverseTransform(Point2D p) {
    Point2D shifted(p.x - mOffset.x, p.y - mOffset.y);          // step 1: undo the shift
    float pX = std::cos(-mRotation)*shifted.x - std::sin(-mRotation)*shifted.y;
    float pY = std::sin(-mRotation)*shifted.x + std::cos(-mRotation)*shifted.y;
    return Point2D(pX, pY);
}


/**
 * @brief   Sets the translation applied by Transform() to every point drawn through this context.
 * @param   p The object's position in screen (world) coordinates.
 * Used by GameEngine only (friend). A GUI context never calls this, so it stays at (0, 0).
 * AI-ASSISTED: explained | AI did: described the offset/rotation design and wrote this comment text | I did: wrote the function
 */
void DrawContext::SetContextOffset(Point2D p) {
    mOffset = p;
}

/**
 * @brief   Sets the rotation applied by Transform() to every point drawn through this context.
 * @param   rotation Angle in radians, counter-clockwise on a standard axis (clockwise on screen, since y grows down).
 * AI-ASSISTED: explained | AI did: described the offset/rotation design and wrote this comment text | I did: wrote the function
 */
void DrawContext::SetContextRotation(float rotation) {
    mRotation = rotation;
}

/**
 * @brief   Transforms a rectangle to screen space and returns its axis-aligned bounding box.
 * @param   r A rectangle in local coordinates.
 * @return  The smallest axis-aligned Rect containing all four transformed corners. When the rotation is not
 *          a multiple of 90 degrees the box is larger than the rotated shape, so use it only as a fast first check.
 * AI-ASSISTED: skeleton | AI did: gave the corner structure with blanks, pointed out the swapped x/y and the box(b, b) misuse; wrote this comment text | I did: filled the blanks
 */
Rect DrawContext::Transform(Rect r) {
    Point2D a = Transform(r.topLeft); //top-left corner
    Point2D b = Transform(Point2D(r.topLeft.x + r.width, r.topLeft.y)); //top-right corner
    Point2D c = Transform(Point2D(r.topLeft.x, r.topLeft.y + r.height));   // bottom-left corner
    Point2D d = Transform(Point2D(r.topLeft.x + r.width, r.topLeft.y + r.height));   // bottom-right corner
    Rect box(a, a);
    box |= b;
    box |= c;
    box |= d;
    return box;
}

}  // namespace CMPUT350
