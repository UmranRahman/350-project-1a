#ifndef BULLET_H
#define BULLET_H

#include "CollisionObject.h"
#include "GameContext.h"
#include <memory>
#include "DrawContext.h"
#include <vector>

class Bullet : public CMPUT350::CollisionObject
{
public:
	Bullet(CMPUT350::Point2D location, CMPUT350::Point2D heading, bool player, std::weak_ptr<CMPUT350::GameObject> parent);
	bool IsPlayerBullet();
	void Update(CMPUT350::GameContext *context) override;
	void LateUpdate(CMPUT350::GameContext *context) override;
	void RenderForeground(CMPUT350::GameContext *context) override;
	bool IsAlive() const override;
	void Kill() override;
	const std::vector<CMPUT350::Shape> &GetShapes() override;
	void CollisionEnter(const std::shared_ptr<CMPUT350::CollisionObject> &obj) override;
	void CollisionEnter(const std::shared_ptr<CMPUT350::CollisionObject> &obj, const CMPUT350::Point2D &collisionPoint) override;
	const CMPUT350::Rect &GetBounds() override;
	
	CMPUT350::Point2D GetLocation() const override;
	float GetRotation() const override;
private:
    void UpdateShape();

    CMPUT350::Point2D mLoc;
    CMPUT350::Point2D mHeading;
    CMPUT350::Rect mBounds;
    std::vector<CMPUT350::Shape> mShapes;
    bool mPlayer;
    bool mAlive = true;
    bool mHit = false;
    CMPUT350::Point2D mHitPoint;
    std::weak_ptr<CMPUT350::GameObject> mParent;
    int mHitFrames = 0;
};

#endif // BULLET_H
