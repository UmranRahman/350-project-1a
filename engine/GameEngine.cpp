#include "GameEngine.h"

#include "GameObject.h"
#include <algorithm>
#include <optional>
#include <cstdio>

/// @brief
namespace CMPUT350 {
#include "FontData.h"



GameEngine::GameEngine(unsigned int width, unsigned int height, const std::string& name) {
    mWindow = std::make_shared<sf::RenderWindow>(sf::VideoMode({width, height}), name);
    mWindow->setFramerateLimit(30);

    mFont = std::make_shared<sf::Font>();
    
    // Sample font loading code
    if (!mFont->openFromMemory(&_font, _font_len)) {
        fprintf(stderr, "WARNING: Font did not load.\n");
    }

    mDrawContext = std::make_unique<DrawContext>(mWindow, mFont);

}

GameEngine::~GameEngine() {
    // Cleanup resources
    mWindow->close();
}

void GameEngine::AddGameObject(std::shared_ptr<GameObject> gameObject) {
    mPendingObjects.push_back(gameObject);
}

/**
 * @brief Handles window-close and key events; key presses are passed to every object. 
 * @param context Passed through to HandleKeyEvent.
 * */
void GameEngine::ProcessEvents(GameContext *context) {
    while (const std::optional event = mWindow->pollEvent())
    {
        if (event->is<sf::Event::Closed>()) {
            mWindow->close();
        }
        else if (const auto* keyPressed = event->getIf<sf::Event::TextEntered>()) {
            // use keyPressed->unicode to get character
            // AI-assisted: Used to get template for keypressed events
            if (keyPressed->unicode < 128) {
                for (auto &odject : mGameObjects) {
                    odject->HandleKeyEvent(context, static_cast<char>(keyPressed->unicode));
                }
            }
        }
    }
}

// AI-assisted: pair loop and bounds-copy idea explained by AI
void GameEngine::ProcessCollisions(GameContext *) {
    std::vector<std::shared_ptr<CollisionObject>> collided;
    for (auto &object: mGameObjects){
        auto ball = std::dynamic_pointer_cast<CollisionObject>(object);
        if (ball) {
            collided.push_back(ball);
        }
    }
    for (int i = 0; i < collided.size(); ++i) {
        for (int j = 0; j < collided.size(); ++j) {
        Rect a = collided[i]->GetBounds();                // COPIES, not references (demo reuses one static Rect)
        Rect b = collided[j]->GetBounds();
        if (Overlaps(a, b)) {
            collided[i]->CollisionEnter(collided[j]);
            collided[j]->CollisionEnter(collided[i]);
        }
    }
    }
}

/**
 * @method Run
 * @arguments None
 * @description Gives control to the game engine. Will not return until the game window is closed or
 * all objects have been destroyed.
 */
void GameEngine::Run() {
    GameContext context{this, mDrawContext.get()};
    while (mWindow->isOpen())  // window is open
    {
        // 0. Remove any objects that are now dead
        // AI-assisted: erase-remove idiom explained by AI
        mGameObjects.erase(
            std::remove_if(
                mGameObjects.begin(),
                mGameObjects.end(),
                [](const std::shared_ptr<GameObject> &object) {
                    return !object->IsAlive();
                }
            ), mGameObjects.end()
        );
        // 1. Activate and initialize any objects added during the last frame
        // AI-assisted: swap idea explained by AI
        std::vector<std::shared_ptr<GameObject>> batch;
        batch.swap(mPendingObjects);
        for (auto &object : batch) {
            mGameObjects.push_back(object);
            object->Initialize(&context);
        }

        // 2. Process events
        ProcessEvents(&context);
        
        
        // 3. Update game objects
        for (auto &object : mGameObjects) {
            object->Update(&context);
        }

        // 4. Process collision events
        ProcessCollisions(&context);

        // 5. Late updates
        for (auto &object : mGameObjects) {
            object->LateUpdate(&context);
        }

        // Clear window
        mWindow->clear();

        // 6. Render background
        // AI-assisted: dynamic_pointer_cast explained by AI
        for (auto &object : mGameObjects) {
            auto graphic = std::dynamic_pointer_cast<GraphicsObject>(object);
            if (graphic) {
                graphic->RenderBackground(&context);
            }
        }

        // 7. Render foreground
        for (auto &object : mGameObjects) {
            auto graphic = std::dynamic_pointer_cast<GraphicsObject>(object);
            if (graphic) {
                graphic->RenderForeground(&context);
            }
        }

        // Actually render to window
        mWindow->display();
    }
}

// Sample code for processing events

// bool GameEngine::ProcessEvents(GameContext *context)
//{
//	while (const std::optional event = mWindow->pollEvent())
//	{
//		if (event->is<sf::Event::Closed>())
//		{
//		}
//		else if (event->is<sf::Event::Resized>())
//		{
//		}
//		else if (const auto* keyPressed = event->getIf<sf::Event::TextEntered>())
//		{
//			// use keyPressed->unicode to get character
//		}
//	}
// }

}  // namespace CMPUT350
