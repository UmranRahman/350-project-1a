#include "Bullet.h"

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

void Bullet::RenderForeground(CMPUT350::GameContext* context)
{
    if(mHit) {
        context->ScreenContext->DrawCircle(mHitPoint, 4, CMPUT350::Colors::yellow);
    } else {
        CMPUT350::RGBColor color = mPlayer ? CMPUT350::Colors::green : CMPUT350::Colors::red;
        context->ScreenContext->DrawLine(mHeading * -1, CMPUT350::Point2D(0, 0), 3, color);
    }
}

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

void Bullet::Kill()
{
    mAlive = false;
}

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

