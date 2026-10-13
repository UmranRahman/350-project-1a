#ifndef STARS_H
#define STARS_H

#include "GraphicsObject.h"
#include <random>

class Stars : public CMPUT350::GraphicsObject
{
public:
    Stars(int numStars, CMPUT350::Rect bounds);
    void RenderBackground(CMPUT350::GameContext* context) override;
    
    //AI-ASSISTED: provided | AI did: told me the two return values and where the declarations go | I did: typed them
    CMPUT350::Point2D GetLocation() const override { return CMPUT350::Point2D(0.0); }
    float GetRotation() const override { return 0.0f; }

private:
    std::vector<CMPUT350::Point2D> mStarPositions;
    CMPUT350::Rect mBounds;
    std::random_device rd;
    std::mt19937 gen;
};


#endif // STARS_H
