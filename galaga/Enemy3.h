#ifndef ENEMY3_H
#define ENEMY3_H
#include "Enemy.h"

class Enemy3 : public Enemy
{
public:
    Enemy3(CMPUT350::Point2D home, const std::vector<CMPUT350::Point2D> &path);
    const char* Name() const override;
    bool Attack(CMPUT350::GameContext* context, const CMPUT350::Point2D &target) override;
    void RenderForeground(CMPUT350::GameContext *context) override;
};
#endif