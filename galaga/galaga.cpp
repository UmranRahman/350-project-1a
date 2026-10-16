#include "Galaga.h"

#include "Enemy.h"
#include "Enemy1.h"
#include "Enemy2.h"
#include "Enemy3.h"
#include "Stars.h"

#include "EngineView.h"
#include "NotificationManager.h"
#include "Keyboard.h"

#include <string>
#include <vector>

/**
 * @brief Creates the game manager in the waiting state and stores the screen size.
 * @param width Screen width in pixels.
 * @param height Screen height in pixels.
 * AI-ASSISTED: skeleton | AI did: gave the members and constructor with blanks | I did: filled the blanks
 */
Galaga::Galaga(int width, int height) : mWidth(width), mHeight(height)
{
    
}

/**
 * @brief Installs the key notifications and registers Galaga for coin, start, player-died and enemy-died messages.
 * @param context Engine context (engine view, notification manager, CurrObject).
 * AI-ASSISTED: provided | AI did: gave the arrow-key installs, mSelf and the Register calls | I did: wrote the WASD/space/coin/start installs
 */
void Galaga::Initialize(CMPUT350::GameContext* context)
{
    //WASD
    context->mEngineView->InstallKeyDownNotification(CMPUT350::Key::A, "MoveLeft");
    context->mEngineView->InstallKeyUpNotification(CMPUT350::Key::A, "StopLeft");
    context->mEngineView->InstallKeyDownNotification(CMPUT350::Key::D, "MoveRight");
    context->mEngineView->InstallKeyUpNotification(CMPUT350::Key::D, "StopRight");

    //direction pad
    context->mEngineView->InstallKeyDownNotification(CMPUT350::Key::Left, "MoveLeft");
    context->mEngineView->InstallKeyUpNotification(CMPUT350::Key::Left, "StopLeft");
    context->mEngineView->InstallKeyDownNotification(CMPUT350::Key::Right, "MoveRight");
    context->mEngineView->InstallKeyUpNotification(CMPUT350::Key::Right, "StopRight");

    context->mEngineView->InstallKeyDownNotification(CMPUT350::Key::Space, "Fire");

    mSelf = context->CurrObject;
    context->mNotificationManager->Register(context->CurrObject, "Coin");
    context->mNotificationManager->Register(context->CurrObject, "Start");
    context->mEngineView->InstallKeyDownNotification(CMPUT350::Key::Num5, "Coin");
    context->mEngineView->InstallKeyDownNotification(CMPUT350::Key::Num1, "Start");

    context->mNotificationManager->Register(context->CurrObject, "PlayerDied");

    for (std::string name : {"Enemy1", "Enemy2", "Enemy3"}) {
        context->mNotificationManager->Register(context->CurrObject, name + "DiedFormation");
        context->mNotificationManager->Register(context->CurrObject, name + "DiedAttack");
    }

    context->mEngineView->AddGameObject(std::make_shared<Stars>(151, CMPUT350::Rect(0.0f, 0.0f, static_cast<float>(mWidth), static_cast<float>(mHeight))));
}

/**
 * @brief Runs the game state machine: startup adds the player, waves spawn 5 batches of 8, gameplay launches an attacker every 90 frames and ends the level when no enemies are left.
 * @param context Engine context (current frame, engine view).
 * AI-ASSISTED: skeleton | AI did: gave the frame-counting idea, state structure and attacker loop with blanks; pointed out missing returns after ChangeState, the respawn placement and the death-frame timing | I did: filled the blanks and fixed the missing braces
 */
void Galaga::Update(CMPUT350::GameContext* context)
{
    if (mStateStartFrame < 0)
        mStateStartFrame = context->currentFrame;
    int elapsed = context->currentFrame - mStateStartFrame;

    if (mState == State::kStartup)
    {
        if (elapsed == 0 && mPlayer.expired()) {
            auto p = std::make_shared<Player>(CMPUT350::Point2D(mWidth / 2.0f, mHeight - 60.0f));
            mPlayer = p;
            context->mEngineView->AddGameObject(p);
        }
        //if (elapsed == 0)
            //context->mEngineView->AddGameObject(std::make_shared<Player>(CMPUT350::Point2D(mWidth / 2.0f, mHeight - 60.0f)));
        if (elapsed >= 90) {
            mWave = 0;
            ChangeState(State::kWaves);
            return;
        }
    }
    if (mState == State::kWaves) {
        if (mWave < 5 && elapsed == mWave * 90) {
            SpawnWave(context);
            ++mWave;
        }
        if (mWave >= 5 && elapsed >= 90*6) {
            ChangeState(State::kGamePlay);
            return;

        }
    }
    if (mState == State::kGamePlay)
    {
        if (mEnemiesAlive <= 0)
        {
            ++mLevel;
            ChangeState(State::kStartup);
            return;

        }
        if (elapsed > 0 && elapsed % 90 == 0) {
            auto player = mPlayer.lock();
            if (player != nullptr)
            {
                for (auto it = context->mEngineView->begin(); it != context->mEngineView->end(); ++it)
                {
                    auto enemy = std::dynamic_pointer_cast<Enemy>(*it);
                    if (enemy != nullptr && enemy->Attack(context, player->GetLocation()))
                        break;
                }
            }
        }
        if ((mState == State::kWaves || mState == State::kGamePlay) && mPlayer.expired())
        {
            if (mLives <= 0)
            {
                ChangeState(State::kGameOver);
                return;
            }
            if (mDeathFrame < 0)
                mDeathFrame = context->currentFrame;
            if (context->currentFrame - mDeathFrame >= 60)
            {
                auto p = std::make_shared<Player>(CMPUT350::Point2D(mWidth / 2.0f, mHeight - 60.0f));
                mPlayer = p;
                context->mEngineView->AddGameObject(p);
                mDeathFrame = -1;
            }
        }
    }
    if (mState == State::kGameOver)
    {
        if (elapsed == 0)
        {
            for (auto it = context->mEngineView->begin(); it != context->mEngineView->end(); ++it)
            {
                if (std::dynamic_pointer_cast<Enemy>(*it) != nullptr)
                    (*it)->Kill();
            }
            mEnemiesAlive = 0;
        }
        if (elapsed >= 150)
        {
            mScore = 0; mLives = 3; mLevel = 1;
            ChangeState(State::kWaiting);
            return;
        }
    }
}

/**
 * @brief Draws the GUI: insert-coin prompt and credits, the level number, the score and the lives.
 * @param context Engine context (GUI draw context).
 * AI-ASSISTED: skeleton | AI did: gave the draw calls with blanks | I did: filled the blanks
 */
void Galaga::RenderUI(CMPUT350::GameContext* context)
{
    if (mState == State::kWaiting)
    {
        context->GUIContext->DrawText("INSERT COIN (5)", 30, CMPUT350::Point2D(mWidth / 2 - 140, mHeight / 2), CMPUT350::Colors::white);
        context->GUIContext->DrawText("CREDITS: " + std::to_string(mCredits), 24, CMPUT350::Point2D(20, mHeight - 40), CMPUT350::Colors::yellow);
    }
    if (mState == State::kStartup) {
        context->GUIContext->DrawText("LEVEL " + std::to_string(mLevel), 40, CMPUT350::Point2D(mWidth / 2 - 90, mHeight / 2), CMPUT350::Colors::yellow);
    }
    if (mState != State::kWaiting) {
        context->GUIContext->DrawText("SCORE " + std::to_string(mScore), 24, CMPUT350::Point2D(20, 20), CMPUT350::Colors::white);
    }
    if (mState != State::kWaiting) {
        context->GUIContext->DrawText("LIVES " + std::to_string(mLives), 24, CMPUT350::Point2D(20, 50), CMPUT350::Colors::white);
    }
    if ((mState == State::kWaves || mState == State::kGamePlay) && mPlayer.expired()){
        context->GUIContext->DrawText("LIVES LEFT: " + std::to_string(mLives), 40, CMPUT350::Point2D(mWidth / 2 - 150, mHeight / 2), CMPUT350::Colors::yellow);
    }
    if (mState == State::kGameOver) {
        context->GUIContext->DrawText("GAME OVER", 50, CMPUT350::Point2D(mWidth / 2 - 150, mHeight / 2), CMPUT350::Colors::red);
    }
}

/**
 * @brief Handles coin, start, player-died (lives) and enemy-died (score and alive count) messages.
 * @param key The notification message.
 * AI-ASSISTED: skeleton | AI did: gave the Start condition and the death-message branch with blanks | I did: wrote the Coin branch, filled the blanks and fixed the comparison mistakes
 */
void Galaga::ReceiveNotification(const std::string& key)
{
    if (key == "Coin"){
        mCredits += 1;
    }
    else if (key == "Start"){
        if (mState == State::kWaiting && mCredits >= 1) {
            mCredits -= 1;
            ChangeState(State::kStartup);
        }
    }
    else if (key == "PlayerDied"){
        --mLives;
    }
    else if (key.find("Died") != std::string::npos)
    {
        --mEnemiesAlive;
        int points = 50;
        if (key.rfind("Enemy2", 0) == 0) points = 100;
        if (key.rfind("Enemy3", 0) == 0) points = 75;
        if (key.find("Attack") != std::string::npos)
            points *= 2;
        mScore += points;
    }
    
}

/**
 * @brief Switches to a new state and flags that the state's start frame must be recorded on the next Update.
 * @param s The state to switch to.
 * AI-ASSISTED: skeleton | AI did: described the -1 trick | I did: wrote the function
 */
void Galaga::ChangeState(State s){
    mState = s;
    mStateStartFrame = -1;
}

/**
 * @brief Spawns one batch of 8 enemies with entry Bezier paths; the enemy type depends on the batch number.
 * @param context Engine context (engine view).
 * AI-ASSISTED: skeleton | AI did: gave the loop and curve with blanks | I did: filled the blanks
 */
void Galaga::SpawnWave(CMPUT350::GameContext *context)
{
    for (int i = 0; i < 8; ++i)
    {
        float x = 64.0f + i * 80;
        float y = 100.0f + mWave * 50;
        CMPUT350::Point2D home(x, y);
        std::vector<CMPUT350::Point2D> path = {
            CMPUT350::Point2D(x, -30), CMPUT350::Point2D(x, 300),
            CMPUT350::Point2D(mWidth / 2.0f, 400), home };

        if (mWave % 3 == 0)
            context->mEngineView->AddGameObject(std::make_shared<Enemy1>(home, path));
        else if (mWave % 3 == 1)
            context->mEngineView->AddGameObject(std::make_shared<Enemy2>(home, path));
        else
            context->mEngineView->AddGameObject(std::make_shared<Enemy3>(home, path));
        ++mEnemiesAlive;
    }
}