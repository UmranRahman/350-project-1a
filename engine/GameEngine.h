
#ifndef GAMEENGINE_H
#define GAMEENGINE_H

namespace CMPUT350 {
}

#include "EngineView.h"
#include "GameObject.h"
#include "MathUtil.h"
#include <SFML/Graphics.hpp>

#include "DrawContext.h"
#include "GameContext.h"
#include "CollisionObject.h"
#include <memory>
#include <vector>
#include <string>

#include "NotificationManager.h"

namespace CMPUT350 {

class GameEngine : public EngineView {
public:
    GameEngine(unsigned int width, unsigned int height, const std::string& name);
    ~GameEngine();

    GameEngine(const GameEngine&) = delete;             // Prevent copy-construction
    GameEngine(GameEngine&&) = delete;                  // Prevent move-construction
    GameEngine& operator=(const GameEngine&) = delete;  // Prevent assignment
    GameEngine& operator=(GameEngine&&) = delete;       // Prevent move-assignment

    void AddGameObject(std::shared_ptr<GameObject> gameObject) override;

    void Run();

private:
    std::shared_ptr<sf::RenderWindow> mWindow;
    std::shared_ptr<sf::Font> mFont;
    std::unique_ptr<DrawContext> mDrawContext;
    
    std::unique_ptr<DrawContext> mGUIDrawContext;
    int mFrame = 0;
    NotificationManager mNotificationManager;

    std::vector<std::shared_ptr<GameObject>> mGameObjects;
    std::vector<std::shared_ptr<GameObject>> mPendingObjects;

    void ProcessEvents(GameContext *context);
    void ProcessCollisions(GameContext *);
};



}  // namespace CMPUT350

#endif  // GAMEENGINE_H
