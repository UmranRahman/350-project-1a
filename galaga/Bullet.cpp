#include "Bullet.h"

/**
 * @brief   Creates a bullet.
 * @param   location Starting world position.
 * @param   heading Movement per frame.
 * @param   player true for a player bullet (green), false for an enemy bullet (red).
 * @param   parent Shooter; the bullet dies when it expires and never collides with it.
 * AI-ASSISTED: provided | AI did: gave the constructor and initializer list; wrote this comment | I did: typed it and adapted names
 */
Bullet::Bullet(CMPUT350::Point2D location, CMPUT350::Point2D heading, bool player, std::weak_ptr<CMPUT350::GameObject> parent)
    : mLoc(location), mHeading(heading), mPlayer(player), mParent(parent)
{
    UpdateShape();
}

bool Bullet::IsPlayerBullet()
{
    return mPlayer;
}

/*void Bullet::Initialize(CMPUT350::GameContext* context)
{
}*/

/**
 * @brief   Moves the bullet by its heading each frame; after a hit it shows for 3 frames then dies.
 *          Also dies when it leaves the screen.
 * AI-ASSISTED: explained | AI did: described the hit-frame counter and corrected my first version, which killed the bullet immediately; wrote this comment | I did: wrote the function
 */
void Bullet::Update(CMPUT350::GameContext* context)
{
    if(mHit) {
        ++mHitFrames;
        if (mHitFrames >= 3){
            mAlive = false;
        }
        return;
    }
    mLoc += mHeading;
    if (mLoc.y < 0 || mLoc.y > 1024) {
        mAlive = false;
    }
}

void Bullet::LateUpdate(CMPUT350::GameContext* context)
{
}

//bool Bullet::HandleKeyEvent(CMPUT350::GameContext* context, char key) {}

/*void Bullet::RenderBackground(CMPUT350::GameContext* context)
{
}*/


/**
 * @brief   Draws a line trailing the bullet, or a circle at the collision point after a hit.
 * AI-ASSISTED: explained | AI did: described the design and the colour-by-owner ternary; wrote this comment | I did: wrote the function
 */
void Bullet::RenderForeground(CMPUT350::GameContext* context)
{
    if(mHit) {
        context->ScreenContext->DrawCircle(mHitPoint, 4, CMPUT350::Colors::yellow);
    } else {
        CMPUT350::RGBColor color = mPlayer ? CMPUT350::Colors::green : CMPUT350::Colors::red;
        context->ScreenContext->DrawLine(mHeading * -1, CMPUT350::Point2D(0, 0), 3, color);
    }
}

/**
 * @brief   Records a hit (ignoring its parent and other bullets) and removes its line shape.
 * @param   obj The object hit.
 * AI-ASSISTED: explained | AI did: described the ignore-parent and ignore-bullet rules and the nullptr fix; wrote this comment | I did: wrote both overloads
 */
void Bullet::CollisionEnter(const std::shared_ptr<CMPUT350::CollisionObject>& obj)
{
    if (obj.get() == mParent.lock().get()) {
        return;
    }
    if (std::dynamic_pointer_cast<Bullet>(obj) != nullptr) {
        return;
    }
    mHit = true;
    mHitPoint = CMPUT350::Point2D(0,0);
    mShapes.clear();
}

/**
 * @brief   Marks the bullet dead so the engine removes it.
 * AI-ASSISTED: explained | AI did: said Kill should clear the alive flag; wrote this comment | I did: wrote it
 */
void Bullet::Kill()
{
    mAlive = false;
}

/**
 * @brief   True while the bullet's parent exists and it has not expired.
 * AI-ASSISTED: provided | AI did: gave the expression; wrote this comment | I did: typed it
 */
bool Bullet::IsAlive() const
{
    return mAlive && !mParent.expired();
}

const CMPUT350::Rect& Bullet::GetBounds()
{
    return mBounds;
}

void Bullet::UpdateShape()
{
    mShapes.clear();
    mShapes.push_back(CMPUT350::Line(mHeading * -1, CMPUT350::Point2D(0, 0)));
    mBounds = CMPUT350::Rect(mHeading * -1, CMPUT350::Point2D(0, 0));
}

CMPUT350::Point2D Bullet::GetLocation() const { return mLoc; }
float Bullet::GetRotation() const { return 0.0f; }
const std::vector<CMPUT350::Shape> &Bullet::GetShapes() { return mShapes; }

/**
 * @brief   Records a hit (ignoring its parent and other bullets) and removes its line shape.
 * @param   obj The object hit.
 * @param   collisionPoint First contact point in the bullet's local space (zero for the overload without a point).
 * AI-ASSISTED: explained | AI did: described the ignore-parent and ignore-bullet rules and the nullptr fix; wrote this comment | I did: wrote both overloads
 */
void Bullet::CollisionEnter(const std::shared_ptr<CMPUT350::CollisionObject> &obj, const CMPUT350::Point2D &collisionPoint) {
    if (obj.get() == mParent.lock().get()) {
        return;
    }
    if (std::dynamic_pointer_cast<Bullet>(obj) != nullptr) {
        return;
    }
    mHit = true;
    mHitPoint = collisionPoint;
    mShapes.clear();
}

