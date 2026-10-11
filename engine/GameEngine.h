
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
#include <unordered_map>
#include "Keyboard.h"

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


    // Key notification installers and object iterators (EngineView interface, 1b handout).
    // AI-ASSISTED: explained | AI did: said these six pure virtuals from the handout must be overridden here with const/override on the iterators and the same signatures as EngineView | I did: wrote the declarations
    void InstallKeyDownNotification(int key, const std::string &notification) override;
    void InstallKeyUpNotification(int key, const std::string &notification) override;
    const_iterator cbegin() const override;
    const_iterator cend() const override;
    const_iterator begin() const override;
    const_iterator end() const override;


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

    std::unordered_map<int, std::vector<std::string>> mKeyDownNotifications;
    std::unordered_map<int, std::vector<std::string>> mKeyUpNotifications;
};



}  // namespace CMPUT350


#endif  // GAMEENGINE_H