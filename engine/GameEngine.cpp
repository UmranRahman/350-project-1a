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
    if (!mFont->openFromMemory(&_font, _font_len))
    {
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
        
        // 1. Activate and initialize any objects added during the last frame

        // 2. Process events
        ProcessEvents(&context);
        mWindow->clear(sf::Color::Black);
        mWindow->display();
        // 3. Update game objects

        // 4. Process collision events

        // 5. Late updates

        // Clear window

        // 6. Render background

        // 7. Render foreground

        // Actually render to window
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
