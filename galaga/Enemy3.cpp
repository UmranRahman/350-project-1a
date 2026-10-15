#include "Enemy3.h"
#include "DrawContext.h"

/**
 * @brief Creates an Enemy3 with 1 health.
 * @param home Formation position.
 * @param path Bezier control points for flying in.
 * AI-ASSISTED: skeleton | AI did: gave the constructor with a blank | I did: filled the blank
 */
Enemy3::Enemy3(CMPUT350::Point2D home, const std::vector<CMPUT350::Point2D> &path)
    : Enemy(home, path, 1)
{
}

/**
 * @brief Name used in death notifications.
 * @return "Enemy3"
 * AI-ASSISTED: skeleton | AI did: gave the structure | I did: filled the blank
 */
const char* Enemy3::Name() const { return "Enemy3"; }

/**
 * @brief Builds Enemy3's wide-swing attack curve and switches to attacking. Refuses if not in formation.
 * @param context Unused.
 * @param target Where the dive aims (the player).
 * @return true if the attack started.
 * AI-ASSISTED: skeleton | AI did: gave the structure and the idea of a wide swing | I did: filled the blanks
 */
bool Enemy3::Attack(CMPUT350::GameContext* /*context*/, const CMPUT350::Point2D &target)
{
    if (mState != State::kInFormation)
        return false;
    mAttackPath = { mLoc, mLoc + CMPUT350::Point2D(300, -50), target + CMPUT350::Point2D(-150, 0), mHome };
    mT = 0;
    mState = State::kAttacking;
    return true;
}

/**
 * @brief Draws Enemy3 as a green circle in local space.
 * @param context Engine context (screen draw context).
 * AI-ASSISTED: skeleton | AI did: gave the draw call with a blank | I did: filled the blank
 */
void Enemy3::RenderForeground(CMPUT350::GameContext *context)
{
    context->ScreenContext->DrawCircle(CMPUT350::Point2D(0, 0), 15, CMPUT350::Colors::green);
}