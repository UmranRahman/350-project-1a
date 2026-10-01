#include "DrawContext.h"


// AI-assisted: SFML 3 usage (Text constructor taking a font, outline setters, ConvexShape)
// was learned from AI-provided examples; the functions below are my own adaptations. See README.
namespace CMPUT350 {

DrawContext::DrawContext(std::shared_ptr<sf::RenderWindow> window, std::shared_ptr<sf::Font> font)
    : mWindow(window), mFont(font) {}

void DrawContext::DrawCenteredText(const std::string &text, int pixelSize, Point2D p, RGBColor c) {
    sf::Text label(*mFont);
    label.setString(text);
    label.setCharacterSize(static_cast<unsigned int>(pixelSize));
    label.setFillColor(sf::Color(c.r, c.g, c.b));
    label.setPosition({p.x, p.y});
    mWindow->draw(label);
}

void DrawContext::DrawText(const std::string &text, int pixelSize, Point2D p, RGBColor c) {
    sf::Text label(*mFont);
    label.setString(text);
    label.setCharacterSize(static_cast<unsigned int>(pixelSize));
    label.setFillColor(sf::Color(c.r, c.g, c.b));
    label.setPosition({p.x, p.y});
    mWindow->draw(label);
}

void DrawContext::DrawCircle(Point2D p, float radius, RGBColor c) {
    sf::CircleShape circle(radius);
    sf::Vector2f position({p.x, p.y});
    sf::Vector2f origin(radius, radius);
    circle.setOrigin(origin);
    circle.setPosition(position);
    circle.setFillColor(sf::Color(c.r, c.g, c.b));
    mWindow->draw(circle);
    
}

void DrawContext::DrawRect(Rect r, RGBColor c) {
    sf::RectangleShape rect({r.width, r.height});
    rect.setPosition({r.topLeft.x, r.topLeft.y});
    rect.setFillColor(sf::Color(c.r, c.g, c.b));
    mWindow->draw(rect);
}

void DrawContext::FrameRect(Rect r, float width, RGBColor c) {
    sf::RectangleShape rect({r.width, r.height});
    rect.setPosition({r.topLeft.x, r.topLeft.y});
    rect.setOutlineThickness(width);
    rect.setOutlineColor(sf::Color(c.r, c.g, c.b));
    rect.setFillColor(sf::Color::Transparent);
    mWindow->draw(rect);
}

/**
 * @brief Draws a line between two points with a specified width and color.
 *
 * @param from The starting point of the line (Point2D).
 * @param to The ending point of the line (Point2D).
 * @param width The width of the line in pixels.
 * @param c The color of the line, specified as an RGBColor object.
 *
 * This function calculates the distance and angle between the two points
 * and uses a polygone shape to represent the line. The line is drawn
 * relative to the world offset and rendered onto the associated window.
 */
void DrawContext::DrawLine(Point2D from, Point2D to, float width, RGBColor c) {
    Point2D direction = to - from;
    direction.Normalize();
    //AI-Assisted: the perpendicular-offset idea for a thick line (rotate direction 90 degrees, scale by half the width) was explained by AI; I wrote the corner code. See README.
    Point2D perp(-direction.y, direction.x);
    Point2D offset = perp * (width / 2);

    sf::ConvexShape shape(4);

    Point2D p1 = from + offset;
    shape.setPoint(0, {p1.x, p1.y});
    Point2D p2 = to + offset;
    shape.setPoint(1, {{p2.x, p2.y}});
    Point2D p3 = to - offset;
    shape.setPoint(2, {{p3.x, p3.y}});
    Point2D p4 = from - offset;
    shape.setPoint(3, {{p4.x, p4.y}});
    shape.setFillColor(sf::Color(c.r, c.g, c.b));
    mWindow->draw(shape);
}

int DrawContext::GetWindowWidth() { return mWindow->getSize().x; }

int DrawContext::GetWindowHeight() { return mWindow->getSize().y; }

}  // namespace CMPUT350
