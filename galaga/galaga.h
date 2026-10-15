#ifndef GALAGA_H
#define GALAGA_H
#include "GameObject.h"
#include "GameContext.h"
#include <memory>

class Galaga : public CMPUT350::GameObject
{
public:
    enum class State { kWaiting, kStartup, kWaves, kGamePlay, kGameOver };

    Galaga(int width, int height);

    void Initialize(CMPUT350::GameContext *context) override;
    void Update(CMPUT350::GameContext *context) override;
    void RenderUI(CMPUT350::GameContext *context) override;
    void ReceiveNotification(const std::string &key) override;

private:
    int mWidth;
    int mHeight;
    State mState = State::kWaiting;
    int mCredits = 0;
    int mScore = 0;
    int mLives = 3;
    int mLevel = 1;
    int mWave = 0;
    int mEnemiesAlive = 0;
    int mStateStartFrame = -1;
    std::weak_ptr<CMPUT350::GameObject> mSelf;

    void SpawnWave(CMPUT350::GameContext *context);
    void ChangeState(State s);
};
#endif