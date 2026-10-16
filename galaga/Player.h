#ifndef PLAYER_H
#define PLAYER_H

#include "CollisionObject.h"
#include <vector>
#include "Bullet.h"
#include <memory>

class Player : public CMPUT350::CollisionObject
{
public:
    Player(CMPUT350::Point2D loc);

    // GameObject Functions
    void Initialize(CMPUT350::GameContext* context) override;
    void Update(CMPUT350::GameContext* context) override;
    void LateUpdate(CMPUT350::GameContext* context) override;
    //bool HandleKeyEvent(CMPUT350::GameContext* context, char key) override;
    bool IsAlive() const override;
    void Kill() override;

    // Graphics Object Functions
    void RenderBackground(CMPUT350::GameContext* context) override;
    void RenderForeground(CMPUT350::GameContext* context) override;


    // Collision Object Functions
    void CollisionEnter(const std::shared_ptr<CMPUT350::CollisionObject>& obj) override;
    const CMPUT350::Rect& GetBounds() override;

    CMPUT350::Point2D GetLocation() const override;
    float GetRotation() const override;
    const std::vector<CMPUT350::Shape> &GetShapes() override;

    void ReceiveNotification(const std::string &key) override;

private:
    CMPUT350::Point2D mLoc;
    CMPUT350::Rect mBounds;
    std::vector<CMPUT350::Shape> mShapes;
    int mMoveDir = 0;   // -1 = left, 0 = still, +1 = right

    //For bullets
    std::weak_ptr<CMPUT350::GameObject> mSelf;
    std::weak_ptr<Bullet> mBullets[2];
    bool mWantsToFire = false;
    bool mHit = false;
    bool mAlive = true;


};

#endif
