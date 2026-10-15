#include "Enemy.h"
#include "Bullet.h"
#include "EngineView.h"
#include "Player.h"
#include "NotificationManager.h"

#include <cmath>
#include <algorithm>

Enemy::Enemy(CMPUT350::Point2D home, const std::vector<CMPUT350::Point2D> &path, int health)
    : mLoc(path.front()), mHome(home), mHealth(health),  mPath(path),  mBounds(-15, -15, 30, 30)
{
    mShapes.push_back(mBounds);
}

void Enemy::Initialize(CMPUT350::GameContext* context)
{
    mSelf = context->CurrObject;
}

/**
   * @brief Runs the enemy's state machine once per frame: fly in, hover in formation, attack and shoot.
   * @param context Game context; currentFrame drives the hover and the firing, EngineView adds bullets.
   * AI-ASSISTED: skeleton | AI did: gave the switch structure and the blanks for each state | I did: filled in the blanks
*/
void Enemy::Update(CMPUT350::GameContext* context)
{
    switch (mState)
    {
        case State::kEntering:
            if (Follow(mPath))
            {
                mState = State::kInFormation;
                mRotation = 0;
                mLoc = mHome;
            }
            break;

        case State::kInFormation:
            mLoc.x = mHome.x + 20.0f * std::sin(context->currentFrame * 0.05f);
            break;

        case State::kAttacking:
            if (Follow(mAttackPath)) {
                mState = State::kInFormation;
                mRotation = 0;
                mLoc = mHome;
            }
            if (context->currentFrame % 40 == 0)
            {
                auto b = std::make_shared<Bullet>(mLoc + CMPUT350::Point2D(0, 20), CMPUT350::Point2D(0, 8), false, mSelf);
                context->mEngineView->AddGameObject(b);
            }
            break;
    }
}

/*
//bool Enemy::HandleKeyEvent(CMPUT350::GameContext* context, char key){}

void Enemy::RenderBackground(CMPUT350::GameContext* context)
{
}*/
/*
void Enemy::RenderForeground(CMPUT350::GameContext* context)
{
}*/

/**
 * @brief Applies damage from player bullets, or instant death on touching the player. Ignores enemy bullets and other enemies.
 * @param obj The object collided with.
 * AI-ASSISTED: skeleton | AI did: gave the structure with blanks and the idea of splitting the work between CollisionEnter and LateUpdate | I did: filled the blanks
 */
void Enemy::CollisionEnter(const std::shared_ptr<CMPUT350::CollisionObject>& obj)
{
    auto bullet = std::dynamic_pointer_cast<Bullet>(obj);
    if (bullet != nullptr)
    {
        if (!bullet->IsPlayerBullet())
            return;
        mHealth -= 1;
    }
    else if (std::dynamic_pointer_cast<Player>(obj) != nullptr)
    {
        mHealth = 0;
    }
    return;
}

/**
 * @brief Kills the enemy when its health reaches zero and notifies Galaga with "<Name>DiedFormation" or "<Name>DiedAttack".
 * @param context Engine context (notification manager).
 * AI-ASSISTED: skeleton | AI did: gave the structure with blanks | I did: filled the blanks
 */
void Enemy::LateUpdate(CMPUT350::GameContext* context)
{
    if (mHealth <= 0 && mAlive)
    {
        std::string msg = std::string(Name()) + "Died" + (mState == State::kAttacking ? "Attack" : "Formation");
        Kill();
        context->mNotificationManager->Notify(msg);
    }
}

void Enemy::Kill()
{
    mAlive = false;
}

bool Enemy::IsAlive() const
{
    return mAlive;
}

const CMPUT350::Rect& Enemy::GetBounds()
{
    return mBounds;
}

CMPUT350::Point2D Enemy::GetLocation() const {return mLoc;}

float Enemy::GetRotation() const {return mRotation;}

const std::vector<CMPUT350::Shape> &Enemy::GetShapes() { return mShapes; }

bool Enemy::Follow(const std::vector<CMPUT350::Point2D> &curve)
{
    CMPUT350::Point2D slope = CMPUT350::Bezier::GetSlope(curve, mT);
    float length = std::sqrt(slope.x * slope.x + slope.y * slope.y);
    mT += 6.0f / std::max(length, 0.001f);

    float end = static_cast<float>((curve.size() - 1) / 3);
    if (mT >= end) {
        mLoc = curve.back();
        mT = 0;
        return true;
    }
    mLoc = CMPUT350::Bezier::GetPoint(curve, mT);
    mRotation = std::atan2(slope.y, slope.x) + 1.5708f;
    return false;
}