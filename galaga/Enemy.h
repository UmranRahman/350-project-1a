#ifndef ENEMY_H
#define ENEMY_H

#include "CollisionObject.h"
#include "GameContext.h"
#include <vector>
#include <memory>
#include "Bezier.h"

class Enemy : public CMPUT350::CollisionObject
{
public:
    enum class State { kEntering, kInFormation, kAttacking };

    Enemy(CMPUT350::Point2D loc, const std::vector<CMPUT350::Point2D> &path, int health);
    virtual ~Enemy() = default;

    // GameObject Functions
    void Initialize(CMPUT350::GameContext* context) override;
    void Update(CMPUT350::GameContext* context) override;
    void LateUpdate(CMPUT350::GameContext* context) override;
    //bool HandleKeyEvent(CMPUT350::GameContext* context, char key) override;
    bool IsAlive() const override;
    void Kill() override;

    // Graphics Object Functions
    //void RenderBackground(CMPUT350::GameContext* context) override;
    //void RenderForeground(CMPUT350::GameContext* context) override;


    // Collision Object Functions
    void CollisionEnter(const std::shared_ptr<CMPUT350::CollisionObject>& obj) override;
    const CMPUT350::Rect& GetBounds() override;

    virtual bool Attack(CMPUT350::GameContext *context, const CMPUT350::Point2D &target) = 0;

    CMPUT350::Point2D GetLocation() const override;
    float GetRotation() const override;
    const std::vector<CMPUT350::Shape> &GetShapes() override;

    virtual const char* Name() const = 0; 

protected:
    CMPUT350::Point2D mLoc;
    CMPUT350::Point2D mHome;
    int mHealth;
    State mState = State::kEntering;
    std::vector<CMPUT350::Point2D> mAttackPath;
    float mT = 0;
    std::weak_ptr<CMPUT350::GameObject> mSelf; 

private:
    bool Follow(const std::vector<CMPUT350::Point2D> &curve);

    std::vector<CMPUT350::Point2D> mPath;
    float mRotation = 0;
    CMPUT350::Rect mBounds;
    std::vector<CMPUT350::Shape> mShapes;
    bool mAlive = true; 

};


#endif

