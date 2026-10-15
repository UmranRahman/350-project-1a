#include "Enemy2.h"
#include "DrawContext.h"


/**
 * @brief Creates an Enemy1 with 1 health.
 * @param home Formation position.
 * @param path Bezier control points for flying in.
 * AI-ASSISTED: skeleton | AI did: gave the constructor with a blank | I did: filled the blank
 */
Enemy2::Enemy2(CMPUT350::Point2D home, const std::vector<CMPUT350::Point2D> &path)
    : Enemy(home, path, 2)
{
}

/**
 * @brief Name used in death notifications.
 * @return "Enemy2"
 * AI-ASSISTED: skeleton | AI did: gave the structure | I did: wrote it
 */
const char* Enemy2::Name() const { return "Enemy2"; }


/**
 * @brief Builds Enemy1's attack curve (start, bend, target, home) and switches to attacking. Refuses if not in formation.
 * @param context Unused.
 * @param target Where the dive aims (the player).
 * @return true if the attack started.
 * AI-ASSISTED: skeleton | AI did: gave the structure with blanks and the curve idea | I did: filled the blanks and fixed the mT line
 */
bool Enemy2::Attack(CMPUT350::GameContext* /*context*/, const CMPUT350::Point2D &target)
{
    if (mState != State::kInFormation)
        return false;
    mAttackPath = { mLoc, mLoc + CMPUT350::Point2D(-200, 150), target, mHome };
    mT = 0;
    mState = State::kAttacking;
    return true;
}

/**
 * @brief Draws Enemy1 as a cyan square in local space.
 * @param context Engine context (screen draw context).
 * AI-ASSISTED: skeleton | AI did: gave the draw call | I did: typed it and checked the sizes
 */
void Enemy2::RenderForeground(CMPUT350::GameContext *context)
{
    CMPUT350::RGBColor color = (mHealth >= 2) ? CMPUT350::Colors::magenta : CMPUT350::Colors::blue;
    context->ScreenContext->DrawRect(CMPUT350::Rect(-15, -15, 30, 30), color);
}