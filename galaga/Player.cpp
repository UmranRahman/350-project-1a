#include <cassert>
#include "Player.h"
#include <algorithm>
#include "GameContext.h"
#include "NotificationManager.h"
#include "DrawContext.h"
#include "EngineView.h"


/**
 * @brief   Creates the player at a world position. Its box is stored in local space, centred on (0,0).
 * @param   loc Starting world position (the engine adds this to every local shape).
 * AI-ASSISTED: skeleton | AI did: gave the constructor structure with blanks, explained local vs world coordinates, pointed out the double-offset and pop_back bugs; wrote this comment | I did: filled the blanks and wrote the code
 */
Player::Player(CMPUT350::Point2D loc) : mLoc(loc), mBounds(CMPUT350::Rect(-20, -20, 40, 40))
{
    mShapes.push_back(mBounds);
}

/**
 * @brief   Registers for the movement and fire notifications and keeps a weak pointer to itself.
 * @param   context Engine context; CurrObject is this player's weak pointer.
 * AI-ASSISTED: skeleton | AI did: gave the registration blanks, then corrected the receiver and argument order; wrote this comment | I did: wrote the registrations
 */
void Player::Initialize(CMPUT350::GameContext* context)
{
    context->mNotificationManager->Register(context->CurrObject, "MoveLeft");
    context->mNotificationManager->Register(context->CurrObject, "MoveRight");
    context->mNotificationManager->Register(context->CurrObject, "StopLeft");
    context->mNotificationManager->Register(context->CurrObject, "StopRight");
    context->mNotificationManager->Register(context->CurrObject, "Fire");

    mSelf = context->CurrObject;
}

/**
 * @brief   World position of the player, which is the rotation centre.
 * AI-ASSISTED: skeleton | AI did: gave the structure with blanks; wrote this comment | I did: filled the blanks
 */
void Player::Update(CMPUT350::GameContext* context)
{
    mLoc.x += mMoveDir*7;
    mLoc.x = std::clamp(mLoc.x, 20.0f, 748.0f);

    if (mWantsToFire) {
        mWantsToFire = false;
        for (auto &slot : mBullets) {
            if (slot.expired()) {
                auto b = std::make_shared<Bullet>(mLoc + CMPUT350::Point2D(0, -25),
                            CMPUT350::Point2D(0, -12), true, mSelf);
                slot = b;
                context->mEngineView->AddGameObject(b);
                break;
            }
        }
    }
}

void Player::LateUpdate(CMPUT350::GameContext* context)
{
}

//bool Player::HandleKeyEvent(CMPUT350::GameContext* context, char key) {}


void Player::RenderBackground(CMPUT350::GameContext* context)
{
}

void Player::RenderForeground(CMPUT350::GameContext* context)
{
    context->ScreenContext->DrawRect(CMPUT350::Rect(-20, -20, 40, 40), CMPUT350::Colors::white);
}

void Player::CollisionEnter(const std::shared_ptr<CMPUT350::CollisionObject>& obj)
{
}

void Player::Kill()
{
}

bool Player::IsAlive() const
{
    // TODO: Update code
    return true;
}

/**
 * @brief   Bounding box in local space.
 * AI-ASSISTED: explained | AI did: said to return a member instead of the dummy; wrote this comment | I did: wrote it
 */
const CMPUT350::Rect& Player::GetBounds()
{
    return mBounds;
}


CMPUT350::Point2D Player::GetLocation() const { return mLoc; }

/**
 * @brief   Rotation in radians; always 0 for the player.
 * AI-ASSISTED: skeleton | AI did: gave the structure; wrote this comment | I did: wrote it
 */
float Player::GetRotation() const { return 0.0f; }

/**
 * @brief   Collision shapes in local space (one 40x40 rect).
 * AI-ASSISTED: skeleton | AI did: gave the structure with blanks; wrote this comment | I did: filled the blanks
 */
const std::vector<CMPUT350::Shape> &Player::GetShapes() { return mShapes; }

void Player::ReceiveNotification(const std::string &key) {
    if (key == "MoveLeft"){
        mMoveDir = -1;
    }
    else if (key == "MoveRight"){
        mMoveDir = 1;
    }
    else if (key == "StopLeft"){
        if (mMoveDir == -1){
            mMoveDir = 0;
        }
    }
    else if (key == "StopRight"){
        if (mMoveDir == 1){
            mMoveDir = 0;
        }
    } else if (key == "Fire") {
        mWantsToFire = true;
    }
}
